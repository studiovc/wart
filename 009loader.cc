#undef char
#include<dirent.h>
#define char wchar_t

void loadFile(ascii* filename) {
  ifstream f(filename);
  list<Cell*> cells = buildCells(parse(parenthesize(tokenize(f))));
  for (list<Cell*>::iterator p = cells.begin(); p != cells.end(); ++p)
    rmref(eval(*p));
  for (list<Cell*>::iterator p = cells.begin(); p != cells.end(); ++p)
    rmref(*p);
}

                                  vector<ascii*> sortedFiles(const ascii* dirname, const ascii* ext) {
                                    vector<ascii*> result;
                                    DIR* dir = opendir(dirname);
                                    for (dirent* ent = readdir(dir); ent; ent = readdir(dir)) {
                                      unsigned int n = strlen(ent->d_name), extn = strlen(ext);
                                      if (n < extn) continue;
                                      if (strncmp(&ent->d_name[n-extn], ext, extn)) continue;
                                      if (!isdigit(ent->d_name[0])) continue;
                                      result.push_back(ent->d_name);
                                    }
                                    closedir(dir);
                                    sort(result.begin(), result.end());
                                    return result;
                                  }

void loadFiles(const ascii* ext) {
  vector<ascii*> files = sortedFiles(".", ext);
  cerr << "Before: " << endl;
  for (vector<ascii*>::iterator q = files.begin(); q != files.end(); ++q)
    cerr << *q << endl;
  ascii x[1024];
  strcpy(x, files.front());
  ifstream f(x); //files.front());
  f.peek();
  cerr << "After: " << endl;
  for (vector<ascii*>::iterator q = files.begin(); q != files.end(); ++q)
    cerr << *q << endl;

  for (vector<ascii*>::iterator p = files.begin(); p != files.end(); ++p)
    loadFile(*p);
}
