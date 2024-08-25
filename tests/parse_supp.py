# import re
#
# file_path = "/home/daniel/Documentos/GitHub/CHIP-8_Emulator/minimalraw.log"
#
# try:
# 	with open(file_path, "r") as file:
# 		file_contents = file.read()
# 		print(file_contents)
#
# 		# Extract content between curly braces
# 		extracted_content = re.findall(r'\{([^}]+)\}', file_contents)
# 		print(extracted_content)
#
# except FileNotFoundError:
# 	print("File not found.")
# except IOError:
# 	print("Error reading the file.")

#!/usr/bin/python3

# """
# ValgrindParser
# Copyright 2017 Peter Nirschl <peter.nirschl@gmail.com>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# 	http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# """
#
# import re
# import sys
#
# def usage():
# 	print("Usage: " + sys.argv[0] + " <file_1> ... <file_n>")
#
# def process_file( f , suppressions):
# 	content = f.read()
# 	matches = re.findall('\{[^\}]*\}', content)
# 	for m in matches:
# 		suppressions.add(m)
#
# def main():
# 	if len(sys.argv) < 2:
# 		usage()
# 		sys.exit(-1)
#
# 	suppressions = set()
#
# 	for x in sys.argv[1:]:
# 		with open(x,'r') as f:
# 			process_file(f, suppressions)
# 		f.close()
#
# 	for s in suppressions:
# 		print(s)
#
# if __name__ == "__main__":
# 	main()


libs = set()

with open("lib_sup", "r") as file:
	for line in file:
		libs.add(line.strip())

print(*libs, sep='\n')
