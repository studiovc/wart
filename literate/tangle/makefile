tangle: makefile type_list function_list file_list test_file_list test_list
	g++ -O3 -Wall -Wextra -fno-strict-aliasing boot.cc -o tangle

type_list: boot.cc [0-9]*.cc
	@# assumes struct decl has space before '{'
	@grep -h "^struct .* {" [0-9]*.cc |sed 's/\(struct *[^ ]*\).*/\1;/' > type_list
	@grep -h typedef [0-9]*.cc >> type_list

function_list: boot.cc [0-9]*.cc
	@# assumes function decl has space before '{'
	@grep -h "^[^ #].*) {" [0-9]*.cc |sed 's/ {.*/;/' > function_list

file_list: boot.cc [0-9]*.cc
	@ls [0-9]*.cc |grep -v "\.test\.cc$$" |sed 's/.*/#include "\0"/' > file_list

test_file_list: [0-9]*.test.cc
	@ls [0-9]*.test.cc |sed 's/.*/#include "\0"/' > test_file_list

test_list: [0-9]*.cc
	@grep -h "^[[:space:]]*void test_" [0-9]*.cc |sed 's/^\s*void \(.*\)() {$$/\1,/' > test_list

clean:
	rm -rf tangle* *_list
