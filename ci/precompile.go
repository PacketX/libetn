/**
 * Copyright 2020 PacketX Technology
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

package main

/*
 * Public suffix list at https://publicsuffix.org/list/effective_tld_names.dat
 */

import (
	"bufio"
	"bytes"
	"encoding/binary"
	"flag"
	"fmt"
	"io"
	"net/http"
	"os"
	"regexp"
	"sort"
	"strings"
	"unsafe"

	"golang.org/x/net/idna"
) //end import

const magicNumber = 0x9601042d

const (
	/* These sum of these four values must be no greater than 32. */
	nodesBitsChildren   = 10
	nodesBitsICANN      = 1
	nodesBitsTextOffset = 15
	nodesBitsTextLength = 6

	/* These sum of these four values must be no greater than 32. */
	childrenBitsWildcard = 1
	childrenBitsNodeType = 2
	childrenBitsHi       = 14
	childrenBitsLo       = 14
) //end const

var (
	maxChildren   int
	maxTextOffset int
	maxTextLength int
	maxHi         uint32
	maxLo         uint32
) //end var

const (
	nodeTypeNormal     = 0
	nodeTypeException  = 1
	nodeTypeParentOnly = 2
	numNodeType        = 3
) //end const

const (
	defaultURL    = "https://publicsuffix.org/list/effective_tld_names.dat"
	defaultOutput = "public_suffix_compiled.dat"
) //end const

var (
	labelEncoding = map[string]uint32{}
	labelsList    = []string{}
	labelsMap     = map[string]bool{}
	rules         = []string{}

	/*
	 * validSuffixRE is used to check that the entries in the public suffix
	 * list are in canonical form (after Punycode encoding). Specifically,
	 * capital letters are not allowed.
	 */
	validSuffixRE = regexp.MustCompile(`^[a-z0-9_\!\*\-\.]+$`)

	url    = flag.String("url", defaultURL, "URL of the publicsuffix.org list. If empty, stdin is read instead")
	output = flag.String("output", defaultOutput, "Output filename")
) //end var

func main() {
	if err := main1(); err != nil {
		fmt.Fprintln(os.Stderr, err)
		os.Exit(1)
	} //end if
} //end main

func max(a, b int) int {
	if a < b {
		return b
	} //end if
	return a
} //end max

func u32max(a, b uint32) uint32 {
	if a < b {
		return b
	} //end if
	return a
} //end u32max

func nodeTypeStr(n int) string {
	switch n {
	case nodeTypeNormal:
		return "+"
	case nodeTypeException:
		return "!"
	case nodeTypeParentOnly:
		return "o"
	default:
		panic("unreachable")
	} //end default
} //end nodeTypeStr

func main1() error {
	flag.Parse()

	if nodesBitsTextLength+nodesBitsTextOffset+nodesBitsICANN+nodesBitsChildren > 32 {
		return fmt.Errorf("not enough bits to encode the nodes table")
	} //end if
	if childrenBitsLo+childrenBitsHi+childrenBitsNodeType+childrenBitsWildcard > 32 {
		return fmt.Errorf("not enough bits to encode the children table")
	} //end if

	/* Retrieve data file. */
	var r io.Reader = os.Stdin
	if *url != "" {
		res, err := http.Get(*url)
		if err != nil {
			return err
		} //end if
		if res.StatusCode != http.StatusOK {
			return fmt.Errorf("bad GET status for %s: %d", *url, res.Status)
		} //end if
		r = res.Body
		defer res.Body.Close()
	} //end if

	/* Start parsing. */
	var root node
	icann := false
	br := bufio.NewReader(r)
	for {
		s, err := br.ReadString('\n')
		if err != nil {
			if err == io.EOF {
				break
			} //end if
			return err
		} //end if
		s = strings.TrimSpace(s)
		if strings.Contains(s, "BEGIN ICANN DOMAINS") {
			icann = true
			continue
		} //end if
		if strings.Contains(s, "END ICANN DOMAINS") {
			icann = false
			continue
		} //end if
		if s == "" || strings.HasPrefix(s, "//") {
			continue
		} //end if

		/* Punycode */
		s, err = idna.ToASCII(s)
		if err != nil {
			return err
		} //end if
		if !validSuffixRE.MatchString(s) {
			return fmt.Errorf("bad publicsuffix.org list data: %q", s)
		} //end if

		rules = append(rules, s)

		/* Start with *. or ! */
		nt, wildcard := nodeTypeNormal, false
		switch {
		case strings.HasPrefix(s, "*."):
			s, nt = s[2:], nodeTypeParentOnly
			wildcard = true
		case strings.HasPrefix(s, "!"):
			s, nt = s[1:], nodeTypeException
		} //end switch

		/* Build tree */
		labels := strings.Split(s, ".")
		for n, i := &root, len(labels)-1; i >= 0; i-- {
			label := labels[i]
			n = n.child(label)
			if i == 0 {
				if nt != nodeTypeParentOnly && n.nodeType == nodeTypeParentOnly {
					n.nodeType = nt
				} //end if
				n.icann = n.icann && icann
				n.wildcard = n.wildcard || wildcard
			} //end if
			labelsMap[label] = true
		} //end for
	} //end for

	labelsList = make([]string, 0, len(labelsMap))
	for label := range labelsMap {
		labelsList = append(labelsList, label)
	} //end for
	sort.Strings(labelsList)

	if err := generate(printReal, &root, defaultOutput); err != nil {
		return err
	} //end if

	return nil
} //end main1

func generate(p func(*bytes.Buffer, *node) error, root *node, filename string) error {

	/* Open file */
	file, err := os.Create(filename)
	if err != nil {
		return err
	} //end if
	defer file.Close()

	buf := new(bytes.Buffer)
	if err := p(buf, root); err != nil {
		os.Remove(filename)
		return err
	} //end if

	/* Flush */
	if _, err := file.Write(buf.Bytes()); err != nil {
		os.Remove(filename)
		return err
	} //end if

	return nil
} //end generate

func printReal(buf *bytes.Buffer, n *node) error {
	var intBuf []uint32

	intBuf = append(intBuf, uint32(magicNumber))
	intBuf = append(intBuf, nodesBitsChildren)
	intBuf = append(intBuf, nodesBitsICANN)
	intBuf = append(intBuf, nodesBitsTextOffset)
	intBuf = append(intBuf, nodesBitsTextLength)
	intBuf = append(intBuf, childrenBitsWildcard)
	intBuf = append(intBuf, childrenBitsNodeType)
	intBuf = append(intBuf, childrenBitsHi)
	intBuf = append(intBuf, childrenBitsLo)
	intBuf = append(intBuf, nodeTypeNormal)
	intBuf = append(intBuf, nodeTypeException)
	intBuf = append(intBuf, nodeTypeParentOnly)
	intBuf = append(intBuf, uint32(len(n.children)))

	text := combineText(labelsList)
	if text == "" {
		return fmt.Errorf("internal error: makeText returned no text")
	} //end if

	/* text len */
	intBuf = append(intBuf, uint32(len(text)))

	/* Write current integer array */
	if err := binary.Write(buf, binary.BigEndian, intBuf); err != nil {
		return err
	} //end if

	for _, label := range labelsList {
		offset, length := strings.Index(text, label), len(label)
		if offset < 0 {
			return fmt.Errorf("internal error: could not find %q in text %q", label, text)
		} //end if
		maxTextOffset, maxTextLength = max(maxTextOffset, offset), max(maxTextLength, length)
		if offset >= 1<<nodesBitsTextOffset {
			return fmt.Errorf("text offset %d is too large, or nodeBitsTextOffset is too small", offset)
		} //end if
		if length >= 1<<nodesBitsTextLength {
			return fmt.Errorf("text length %d is too large, or nodeBitsTextLength is too small", length)
		} //end if
		labelEncoding[label] = uint32(offset)<<nodesBitsTextLength | uint32(length)
	} //end for

	/* Write text */
	if err := binary.Write(buf, binary.BigEndian, []byte(text)); err != nil {
		return err
	} //end if

	w := new(bytes.Buffer)
	if err := n.walk(w, assignIndexes); err != nil {
		return err
	} //end if

	/* Calculate node */
	if err := n.walk(w, printNode); err != nil {
		return err
	} //end if

	/* Write node */
	nodeLen := (uint32)(w.Len() / (int)(unsafe.Sizeof(uint32(0))))
	if err := binary.Write(buf, binary.BigEndian, nodeLen); err != nil {
		return err
	} //end if
	if err := binary.Write(buf, binary.BigEndian, w.Bytes()); err != nil {
		return err
	} //end if

	/* Write children */
	if err := binary.Write(buf, binary.BigEndian, uint32(len(childrenEncoding))); err != nil {
		return err
	} //end if
	for _, c := range childrenEncoding {
		if err := binary.Write(buf, binary.BigEndian, c); err != nil {
			return err
		} //end if
	} //end for

	return nil
} //end printReal

type node struct {
	label    string
	nodeType int
	icann    bool
	wildcard bool
	/*
	 * nodesIndex and childrenIndex are the index of this node in the nodes
	 * and the index of its children offset/length in the children arrays.
	 */
	nodesIndex, childrenIndex int
	/*
	 * firstChild is the index of this node's first child, or zero if this
	 * node has no children.
	 */
	firstChild int
	/* children are the node's children, in strictly increasing node label order. */
	children []*node
} //end struct

func (n *node) walk(w io.Writer, f func(w1 io.Writer, n1 *node) error) error {
	if err := f(w, n); err != nil {
		return err
	} //end if
	for _, c := range n.children {
		if err := c.walk(w, f); err != nil {
			return err
		} //end if
	} //end for
	return nil
} //end walk

/*
 * child returns the child of n with the given label. The child is created if
 * it did not exist beforehand.
 */
func (n *node) child(label string) *node {
	for _, c := range n.children {
		if c.label == label {
			return c
		} //end if
	} //end for
	c := &node{
		label:    label,
		nodeType: nodeTypeParentOnly,
		icann:    true,
	} //end struct
	n.children = append(n.children, c)
	sort.Sort(byLabel(n.children))
	return c
} //end child

type byLabel []*node

func (b byLabel) Len() int           { return len(b) }
func (b byLabel) Swap(i, j int)      { b[i], b[j] = b[j], b[i] }
func (b byLabel) Less(i, j int) bool { return b[i].label < b[j].label }

var nextNodesIndex int

/*
 * childrenEncoding are the encoded entries in the generated children array.
 * All these pre-defined entries have no children.
 */
var childrenEncoding = []uint32{
	0 << (childrenBitsLo + childrenBitsHi), /* Without wildcard bit, nodeTypeNormal */
	1 << (childrenBitsLo + childrenBitsHi), /* Without wildcard bit, nodeTypeException */
	2 << (childrenBitsLo + childrenBitsHi), /* Without wildcard bit, nodeTypeParentOnly */
	4 << (childrenBitsLo + childrenBitsHi), /* With wildcard bit, nodeTypeNormal */
	5 << (childrenBitsLo + childrenBitsHi), /* With wildcard bit, nodeTypeException */
	6 << (childrenBitsLo + childrenBitsHi), /* With wildcard bit, nodeTypeParentOnly */
} //end var

var firstCallToAssignIndexes = true

func assignIndexes(w io.Writer, n *node) error {
	if len(n.children) != 0 {
		/* Assign nodesIndex. */
		n.firstChild = nextNodesIndex
		for _, c := range n.children {
			c.nodesIndex = nextNodesIndex
			nextNodesIndex++
		} //end for

		/* The root node's children is implicit. */
		if firstCallToAssignIndexes {
			firstCallToAssignIndexes = false
			return nil
		} //end if

		/* Assign childrenIndex. */
		maxChildren = max(maxChildren, len(childrenEncoding))
		if len(childrenEncoding) >= 1<<nodesBitsChildren {
			return fmt.Errorf("children table size %d is too large, or nodeBitsChildren is too small", len(childrenEncoding))
		} //end if

		n.childrenIndex = len(childrenEncoding)
		lo := uint32(n.firstChild)
		hi := lo + uint32(len(n.children))
		maxLo, maxHi = u32max(maxLo, lo), u32max(maxHi, hi)
		if lo >= 1<<childrenBitsLo {
			return fmt.Errorf("children lo %d is too large, or childrenBitsLo is too small", lo)
		} //end if
		if hi >= 1<<childrenBitsHi {
			return fmt.Errorf("children hi %d is too large, or childrenBitsHi is too small", hi)
		} //end if
		enc := hi<<childrenBitsLo | lo
		enc |= uint32(n.nodeType) << (childrenBitsLo + childrenBitsHi)
		if n.wildcard {
			enc |= 1 << (childrenBitsLo + childrenBitsHi + childrenBitsNodeType)
		} //end if
		childrenEncoding = append(childrenEncoding, enc)
	} else {
		n.childrenIndex = n.nodeType
		if n.wildcard {
			n.childrenIndex += numNodeType
		} //end if
	} //end else
	return nil
} //end assignIndexes

func printNode(w io.Writer, n *node) error {
	for _, c := range n.children {
		encoding := labelEncoding[c.label]
		if c.icann {
			encoding |= 1 << (nodesBitsTextLength + nodesBitsTextOffset)
		} //end if
		encoding |= uint32(c.childrenIndex) << (nodesBitsTextLength + nodesBitsTextOffset + nodesBitsICANN)
		binary.Write(w, binary.BigEndian, encoding)
	} //end for
	return nil
} //end printNode

/*
 * combineText combines all the strings in labelsList to form one giant string.
 * Overlapping strings will be merged: "arpa" and "parliament" could yield
 * "arparliament".
 */
func combineText(labelsList []string) string {
	beforeLength := 0
	for _, s := range labelsList {
		beforeLength += len(s)
	} //end for

	text := crush(removeSubstrings(labelsList))
	return text
} //end combineText

type byLength []string

func (s byLength) Len() int           { return len(s) }
func (s byLength) Swap(i, j int)      { s[i], s[j] = s[j], s[i] }
func (s byLength) Less(i, j int) bool { return len(s[i]) < len(s[j]) }

/*
 * removeSubstrings returns a copy of its input with any strings removed
 * that are substrings of other provided strings.
 */
func removeSubstrings(input []string) []string {
	/* Make a copy of input */
	ss := append(make([]string, 0, len(input)), input...)
	sort.Sort(byLength(ss))

	for i, shortString := range ss {
		/*
		 * For each string, only consider strings higher than it in sort order, i.e.
		 * of equal length or greater.
		 */
		for _, longString := range ss[i+1:] {
			if strings.Contains(longString, shortString) {
				ss[i] = ""
				break
			} //end if
		} //end for
	} //end for

	/* Remove the empty strings */
	sort.Strings(ss)
	for len(ss) > 0 && ss[0] == "" {
		ss = ss[1:]
	} //end for
	return ss
} //end removeSubstrings

/*
 * crush combines a list of strings, taking advantage of overlaps. It returns a
 * single string that contains each input string as a substring.
 */
func crush(ss []string) string {
	maxLabelLen := 0
	for _, s := range ss {
		if maxLabelLen < len(s) {
			maxLabelLen = len(s)
		} //end if
	} //end for

	for prefixLen := maxLabelLen; prefixLen > 0; prefixLen-- {
		prefixes := makePrefixMap(ss, prefixLen)
		for i, s := range ss {
			if len(s) <= prefixLen {
				continue
			} //end if
			mergeLabel(ss, i, prefixLen, prefixes)
		} //end for
	} //end for

	return strings.Join(ss, "")
} //end crush

/*
 * mergeLabel merges the label at ss[i] with the first available matching label
 * in prefixMap, where the last "prefixLen" characters in ss[i] match the first
 * "prefixLen" characters in the matching label.
 * It will merge ss[i] repeatedly until no more matches are available.
 * All matching labels merged into ss[i] are replaced by "".
 */
func mergeLabel(ss []string, i, prefixLen int, prefixes prefixMap) {
	s := ss[i]
	suffix := s[len(s)-prefixLen:]
	for _, j := range prefixes[suffix] {
		/* Empty strings mean "already used." Also avoid merging with self. */
		if ss[j] == "" || i == j {
			continue
		} //end if
		ss[i] += ss[j][prefixLen:]
		ss[j] = ""
		/*
		 * ss[i] has a new suffix, so merge again if possible.
		 * Note: we only have to merge again at the same prefix length. Shorter
		 * prefix lengths will be handled in the next iteration of crush's for loop.
		 * Can there be matches for longer prefix lengths, introduced by the merge?
		 * I believe that any such matches would by necessity have been eliminated
		 * during substring removal or merged at a higher prefix length. For
		 * instance, in crush("abc", "cde", "bcdef"), combining "abc" and "cde"
		 * would yield "abcde", which could be merged with "bcdef." However, in
		 * practice "cde" would already have been elimintated by removeSubstrings.
		 */
		mergeLabel(ss, i, prefixLen, prefixes)
		return
	} //end for
} //end mergeLabel

/*
 * prefixMap maps from a prefix to a list of strings containing that prefix. The
 * list of strings is represented as indexes into a slice of strings stored
 * elsewhere.
 */
type prefixMap map[string][]int

/* makePrefixMap constructs a prefixMap from a slice of strings. */
func makePrefixMap(ss []string, prefixLen int) prefixMap {
	prefixes := make(prefixMap)
	for i, s := range ss {
		/*
		 * We use < rather than <= because if a label matches on a prefix equal to
		 * its full length, that's actually a substring match handled by
		 * removeSubstrings.
		 */
		if prefixLen < len(s) {
			prefix := s[:prefixLen]
			prefixes[prefix] = append(prefixes[prefix], i)
		} //end if
	} //end for

	return prefixes
} //end makePrefixMap
