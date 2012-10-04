void test_transform_handles_dollar_vars() {
  Cell* expr = read(stream("$x"));
  check(isSym(expr));
  checkEq(toString(expr).substr(0, 1), "x");
  rmref(expr);
}