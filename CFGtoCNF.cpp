//Atudore George-Darius - gr 144
//conversie CFG la CNF
#include <iostream>

#include <algorithm>

#include <set>

#include <string>

#include <queue>

using namespace std;

int nrTerm, nrNonTerm, nrProd;
set < char > term;
set < char > nonTerm;
set < string > prod[128];
char start;

//eliminarea productiilor nule
void elimProdNule() {
  bool ok = false;
  while (!ok) {
    ok = true;
    for (int i = 0; i < 128; i++)
      if (!prod[i].empty()) //prod = productiile nevide
        for (auto x: prod[i]) { //pentru fiecare string din productie
          if (x.find('$') != string::npos) //gasesc null in productiile caracterului char(i)
          {
            ok = false;
            if (prod[i].size() == 1) { //verificam daca productia este unica
              for (int j = 0; j < 128; j++) //cautam prin toate caracterele
                if (!prod[j].empty()) //daca e nenula
                  for (auto y: prod[j]) //cautam char(i) printre productii
                    if (y.find(char(i)) != string::npos)
                      if (y.size() >= 2) {
                        y.erase(y.find(char(i)), 1); //sterg caracterul din productii
                      }
              else {
                y.clear();
                y = "$";
              } //daca are un simbol, adaug lambda
            } else { //daca sunt mai multe productii ale lui T elimin productia
              for (int j = 0; j < 128; j++)
                if (!prod[j].empty())
                  for (auto y: prod[j]) //caut T printre productii
                    if (y.find(char(i)) != string::npos)
                      if (y.size() >= 2) { //pentru productiile cu mai mult de 2 simboluri, scot T
                        string temporar = y;
                        temporar.erase(temporar.find(char(i)), 1);
                        prod[j].insert(temporar);
                      }
            }
            set < string > nou;
            for (auto y: prod[i])
              if (y.find('$') == string::npos)
                nou.insert(y);

            prod[i].clear();
            for (auto y: nou)
              prod[i].insert(y);
          }
        }
  }
}

void redenumire() {
  bool ok = false;
  while (!ok) {
    for (int i = 0; i < 128; i++)
      if (!prod[i].empty()) {
        set < string > nou;
        for (auto x: prod[i])
          if (x.size() == 1) {
            for (auto y: nonTerm)
              if (x[0] != y)
                nou.insert(x);
          }
        if (prod[i] == nou)
          ok = true;
        prod[i].clear();
        for (auto y: nou)
          prod[i].insert(y);

      }
  }
}

//eliminarea redenumirilor
void elimRedenumiri() {
  for (int i = 0; i < 128; i++)
    if (!prod[i].empty()) {
      set < string > nou;
      for (auto x: prod[i])
        if (x.size() != 1 || x[0] != char(i))
          nou.insert(x);
      prod[i].clear();
      for (auto x: nou)
        prod[i].insert(x);
    }

  set < string > copie[128];
  bool ok = false;

  while (!ok) {
    ok = true;
    for (int i = 0; i < 128; i++) {
      copie[i] = prod[i];
      prod[i].clear();
      if (!copie[i].empty())
        for (auto x: copie[i])
          if (x.size() == 1 && nonTerm.find(x[0]) != nonTerm.end()) {
            for (auto z: prod[int(x[0])])
              prod[i].insert(z);
            ok = false;
          }
      else
        prod[i].insert(x);
    }
  }

}

void infinite(string a, set < string > & folosite) {
  queue < string > s;
  folosite.insert(a);
  for (auto x: prod[int(a[0])]) {
    for (auto y: x) {
      string temp;
      temp = y;
      if (folosite.find(temp) != folosite.end()) {
        s.push(x);
        break;
      }
      for (auto z: nonTerm)
        if (z == y) {
          string u;
          u = y;
          infinite(u, folosite);
        }
    }
  }
}

//eliminarea productiilor inutile
void elimProdInutile() {
  set < string > verif;
  set < char > elim;
  string s;
  s = start;
  infinite(s, verif);
  for (int i = 0; i < 128; i++)
    if (!prod[i].empty()) {
      char c = i;
      string d;
      d = c;
      if (verif.find(d) == verif.end())
        elim.insert(d[0]);
    }

  for (int i = 0; i < 128; i++)
    if (!prod[i].empty()) {
      int sters = 1;
      for (auto x: prod[i])
        if (x.find(char(i)))
          sters = 0;
      if (sters != 0)
        elim.insert(char(i));
    }

  for (auto x: elim)
    prod[int(x)].clear();

  for (auto x: elim)
    for (int i = 0; i < 128; i++)
      if (!prod[i].empty()) {
        set < string > p;
        for (auto y: prod[i])
          if (y.find(x) == string::npos)
            p.insert(y);
        prod[i].clear();
        for (auto l: p)
          prod[i].insert(l);
      }
}

//adaugarea terminalelor noi
void adNonTerm() {
  set < char > modif;
  for (int i = 0; i < 128; i++)
    if (!prod[i].empty())
      for (auto y: prod[i])
        if (y.size() >= 2)
          for (auto z: y)
            for (auto t: term)
              if (z == t)
                modif.insert(t);
  int cod = 65;
  for (auto x: modif) {
    while (!prod[cod].empty())
      cod++;
    string t;
    t = x;
    nonTerm.insert(char(cod));
    prod[cod].insert(t);
    for (int i = 0; i < 128; i++)
      if (i != cod && !prod[i].empty()) {
        set < string > nou;
        for (auto y: prod[i])
          if ((y.size() < 2) || (y.find(x) == string::npos))
            nou.insert(y);
          else {
            string m;
            for (auto z: y)
              if (z != x)
                m += z;
              else
                m += char(cod);
            nou.insert(m);
          }
        prod[i].clear();
        for (auto n: nou)
          prod[i].insert(n);
      }

  }
}

int nrNonTerminale(string a) {
  int nr = 0;
  for (auto x: nonTerm)
    for (auto y: a)
      if (x == y)
        nr++;
  return nr;
}

void adNonTerm2() {
  set < string > modiff;
  for (int i = 0; i < 128; i++)
    if (!prod[i].empty())
      for (auto x: prod[i])
        if (nrNonTerminale(x) > 2)
          modiff.insert(x);
  int cod = 65;
  for (auto sir: modiff) {
    while (!prod[cod].empty())
      cod++;
    nonTerm.insert(cod);
    string nou;
    string b;
    b = char(cod);
    nou = sir[0] + b;
    string vechi;
    vechi = sir[1];
    for (int i = 2; i < sir.size(); i++)
      vechi += sir[i];
    prod[cod].insert(vechi);
    for (int i = 0; i < 128; i++)
      if (!prod[i].empty()) {
        set < string > q;
        for (auto x: prod[i]) {
          if (x == sir)
            q.insert(nou);
          else
            q.insert(x);
        }
        prod[i].clear();
        for (auto x: q)
          prod[i].insert(x);
      }

  }
}

//citire
void citire() {
  cin >> start;

  //citim nonterminale
  cin >> nrNonTerm;
  for (int i = 0; i < nrNonTerm; i++) {
    char n;
    cin >> n;
    nonTerm.insert(n);
  }

  //citim terminale
  cin >> nrTerm;
  for (int i = 0; i < nrTerm; i++) {
    char t;
    cin >> t;
    term.insert(t);
  }

  //citim productiile
  cin >> nrProd;
  for (int i = 0; i < nrProd; i++) {
    int nr_tranzitii;
    cin >> nr_tranzitii;
    char e;
    cin >> e;
    for (int j = 0; j < nr_tranzitii; j++) {
      string x;
      cin >> x;
      prod[int(e)].insert(x);
    }
  }
}

//afisarea productiilor
void afis() {
  cout << "Productiile finale:\n";
  for (int i = 0; i < 128; i++)
    if (!prod[i].empty()) {
      cout << char(i) << "->";
      for (const auto & x: prod[i])
        cout << x << "|";
      cout << '\n';
    }
}

int main() {
  citire();
  elimProdNule();
  elimRedenumiri();
  elimProdInutile();
  adNonTerm();
  adNonTerm2();
  afis();
  return 0;
}
