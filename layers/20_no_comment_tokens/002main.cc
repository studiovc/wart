//// run unit tests or interactive interpreter

// The design of lisp seems mindful of the following:
//  keep programs as short as possible
//  as programs grow, allow them to be decomposed into functions
//  functions are defined with a recipe for work, and invoked to perform that work
//  functions are parameterized with vars (parameters) and invoked with data (arguments)
//  new functions can be defined at any time
//  functions can invoke other functions before they've been defined
//  functions are data; they can be passed to other functions
//  when an invocation contains multiple functions, make the one being invoked easy to identify
//    almost always the first symbol in the invocation
//  there have to be *some* bedrock primitives, but any primitive can be user-defined in principle
//
// These weren't the reasons lisp was created; they're the reasons I attribute
// to its power.

bool interactive = false;

int main(int argc, unused char* argv[]) {
  if (argc > 1) {
    runTests();
    return 0;
  }

  //// Interactive loop: parse commands from user, evaluate them, print the results
  interactive = true;
  setup();
  loadFiles(".wart");
  cout << "ready! type in an expression, then hit enter twice. ctrl-d exits.\n";
  while (!cin.eof()) {
    cout << "=> " << run(cin) << '\n';
  }
}

//// read: tokenize, segment, parse, build cells
Cell* read(istream& in) {
  return nextCell(in);
}

// In batch mode, evaluate all exprs in input.
// In interactive mode, evaluate all exprs until empty line.
// Return value of last expr.
Cell* run(istream& in) {
  Cell* result = NULL;
  do {
    result = eval(read(in));
  } while (!eof(in) && (!interactive || in.peek() != '\n'));
  return result;
}

// parse a paragraph of expressions until empty line
list<Cell*> readAll(istream& in) {
  list<Cell*> results;
  do {
    results.push_back(read(in));
  } while (!eof(in) && (!interactive || in.peek() != '\n'));
  return results;
}

bool eof(istream& in) {
  in.peek();
  return in.eof();
}



//// test harness

void runTests() {
  runningTests = true;
  pretendRaise = true;  // for death tests
  time_t t; time(&t);
  cerr << "C tests: " << ctime(&t);
  for (unsigned long i=0; i < sizeof(tests)/sizeof(tests[0]); ++i) {
    START_TRACING_UNTIL_END_OF_SCOPE;
    setup();
    (*tests[i])();
    verify();
  }

  pretendRaise = false;
  setup();
  loadFiles(".wart");   // after GC tests
  loadFiles(".test");

  cerr << endl;
  if (numFailures > 0)
    cerr << numFailures << " failure"
         << (numFailures > 1 ? "s" : "")
         << endl;
}

void verify() {
  teardownBindings();
  if (!passed) return;
  if (raiseCount != 0) cerr << raiseCount << " errors encountered" << endl;
}

// helper to read from string
// leaks memory; just for convenient tests
Cell* read(string s) {
  return read(*new stringstream(s));
}

list<Cell*> readAll(string s) {
  stringstream in(s);
  return readAll(in);
}

Cell* run(string s) {
  stringstream in(s);
  return run(in);
}



void setup() {
  setupCells();
  setupCommonSyms();
  raiseCount = 0;
  passed = true;
}
