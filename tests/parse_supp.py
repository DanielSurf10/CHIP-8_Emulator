#!/usr/bin/python3
"""
ValgrindParser
Copyright 2017 Peter Nirschl <peter.nirschl@gmail.com>

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

"""

import re
import sys


def usage():
	print("Usage: " + sys.argv[0] + " <file_1> ... <file_n>")


def process_file(f, suppressions):
	content = f.read()
	matches = re.findall('\\{[^\\}]*\\}', content)
	for m in matches:
		suppressions.add(m)


def main():
	if len(sys.argv) < 2:
		usage()
		sys.exit(-1)

	suppressions = set()

	arq = open("supp_all.supp", "w")

	for x in sys.argv[1:]:
		with open(x, 'r') as f:
			process_file(f, suppressions)
		f.close()

	for s in suppressions:
		print(s, file=arq)

	arq.close()

	arq_supp_all = open("supp_all.supp", "r")
	arq_minimal_supp = open("minimal_supp.supp", "w")

	libs = set()

	for line in arq_supp_all:
		line = line.strip()

		if "obj:" in line:
			libs.add(line)

	for lib in libs:
		print(
			'{\n'
			'\t<LibSDL>\n'
			'\tMemcheck:Leak\n'
			'\t...\n'
			f'\t{lib}\n'
			'\t...\n'
			'}',
			file=arq_minimal_supp
		)


if __name__ == "__main__":
	main()
