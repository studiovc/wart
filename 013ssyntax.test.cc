void test_ssyntax() {
  SsyntaxTemplate s = {L'.', SsyntaxTemplate::MULTIARY, newSym(L"op")};
  ssyntaxTemplates.push_back(s);
  Cell* cons = wartRead(stream(L"a.b")).front();
  check_eq(car(cons), newSym(L"op"));
  check_eq(car(cdr(cons)), newSym(L"a"));
  check_eq(car(cdr(cdr(cons))), newSym(L"b"));
  check_eq(cdr(cdr(cdr(cons))), nil);
  rmref(cons);
  ssyntaxTemplates.clear();
}

void test_ssyntax_skips_floats() {
  SsyntaxTemplate s = {L'.', SsyntaxTemplate::MULTIARY, newSym(L"op")};
  ssyntaxTemplates.push_back(s);
  Cell* val = wartRead(stream(L"2.4")).front();
  check_eq(val, newSym(L"2.4")); // fix when we support floats
  rmref(val);
  ssyntaxTemplates.clear();
}

void test_unary_ssyntax() {
  SsyntaxTemplate s = {L'.', SsyntaxTemplate::UNARY, newSym(L"op")};
  ssyntaxTemplates.push_back(s);
  Cell* sym = wartRead(stream(L"a.b")).front();
  check_eq(sym, newSym(L"a.b")); // doesn't trigger for binary ops
  rmref(sym);

  Cell* cons = wartRead(stream(L".b")).front();
  check_eq(car(cons), newSym(L"op"));
  check_eq(car(cdr(cons)), newSym(L"b"));
  check_eq(cdr(cdr(cons)), nil);
  rmref(cons);
  ssyntaxTemplates.clear();
}

void test_ssyntax_adds_trailing_nil() {
  SsyntaxTemplate s = {L'.', SsyntaxTemplate::MULTIARY, newSym(L"op")};
  ssyntaxTemplates.push_back(s);
  Cell* cons = wartRead(stream(L"a.")).front();
  check_eq(car(cons), newSym(L"op"));
  check_eq(car(cdr(cons)), newSym(L"a"));
  check(isCons(cdr(cdr(cons))));
  check_eq(car(cdr(cdr(cons))), nil);
  check_eq(cdr(cdr(cdr(cons))), nil);
  rmref(cons);
  ssyntaxTemplates.clear();
}

void test_multiary_ssyntax_setup() {
  Cell* def = wartRead(stream(L"ssyntax _._ (op _ _)")).front();
  eval(def); // needn't rmref; returns nil
  Cell* expr = wartRead(stream(L"a.b")).front();
  check_eq(car(expr), newSym(L"op"));
  check_eq(car(cdr(expr)), newSym(L"a"));
  check_eq(car(cdr(cdr(expr))), newSym(L"b"));
  check_eq(cdr(cdr(cdr(expr))), nil);
  rmref(expr);
  ssyntaxTemplates.clear();
  rmref(def);
}
