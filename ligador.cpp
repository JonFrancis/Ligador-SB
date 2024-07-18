#include <bits/stdc++.h>
using namespace std;

vector<pair<string, int>> uso1, def1, uso2, def2;
vector<int> novoObj;
string real1, real2;
unordered_map<string, int> defGlobal;
vector<int> co1, co2;
int fatorCorrecao;

void debug() {
    cout << "Arquivo 1:\n";
    cout << "Uso:\n";
    for (auto& par : uso1) {
        cout << par.first << " " << par.second << "\n";
    }

    cout << "\nDefinição:\n";
    for (auto& par : def1) {
        cout << par.first << " " << par.second << "\n";
    }

    cout << "\nCódigo Objeto:\n";
    for (auto& num : co1) {
        cout << num << " ";
    }
    cout << "\n";

    cout << "\nReal:\n" << real1 << "\n";

    cout << "\nArquivo 2:\n";
    cout << "Uso:\n";
    for (auto& par : uso2) {
        cout << par.first << " " << par.second << "\n";
    }

    cout << "\nDefinição:\n";
    for (auto& par : def2) {
        cout << par.first << " " << par.second << "\n";
    }

    cout << "\nCódigo Objeto:\n";
    for (auto& num : co2) {
        cout << num << " ";
    }
    cout << "\n";

    cout << "\nReal:\n" << real2 << "\n";

    cout << "\nDefinição Global:\n";
    for (const auto& par : defGlobal) {
        cout << par.first << " " << par.second << "\n";
    }
    cout << "\n";

    cout << "\nNovo Obj:\n";
    for (auto& num : novoObj) {
        cout << num << " ";
    }
    cout << "\n";
}

bool carregarArquivo(const string& nomeArquivo, string& conteudo) {
    ifstream arquivo(nomeArquivo);
    if (arquivo.is_open()) {
        string linha;
        while (getline(arquivo, linha)) {
            conteudo += linha + "\n";
        }
        arquivo.close();
        return true;
    } else {
        cout << "Erro ao abrir o arquivo: " << nomeArquivo << endl;
        return false;
    }
}

void processarDados(const string& entrada, vector<pair<string, int>>& uso, vector<pair<string, int>>& def, vector<int>& codigoObjeto, string& real) {
    istringstream iss(entrada);

    string linha;
    bool encontrouDef = false, encontrouReal = false;

    while (getline(iss, linha)) {
        istringstream linhaStream(linha);
        string token;

        if (!encontrouDef) {
            while (linhaStream >> token) {
                if (token == "DEF") {
                    encontrouDef = true;
                    break;
                } else {
                    string nome = token;
                    int numero;
                    if (linhaStream >> numero) {
                        uso.push_back(make_pair(nome, numero));
                    }
                }
            }
        } else if (!encontrouReal) {
            while (linhaStream >> token) {
                if (token == "REAL") {
                    encontrouReal = true;
                    getline(iss, real);  // Captura a linha logo após "REAL"
                    break;
                } else {
                    string nome = token;
                    int numero;
                    if (linhaStream >> numero) {
                        def.push_back(make_pair(nome, numero));
                    }
                }
            }
        } else {
            istringstream numStream(linha);
            int num;
            while (numStream >> num) {
                codigoObjeto.push_back(num);
            }
        }
    }
}

void resolveEnderecos() {
    for (auto& par : uso1) {
        auto it = defGlobal.find(par.first);
        if (it != defGlobal.end()) {
            novoObj[par.second] += it->second;
        }
    }
    for (auto& par : uso2) {
        auto it = defGlobal.find(par.first);
        if (it != defGlobal.end()) {
            novoObj[par.second + fatorCorrecao] += it->second;
        }
    }
}

void resolveReal() {
    for (size_t i = 0; i < real2.size(); ++i) {
        if (real2[i] == '1') {
            novoObj[i + fatorCorrecao] += fatorCorrecao;
        }
    }
}

void salvarNovoObj(const string& nomeArquivo, const vector<int>& obj) {
    ofstream arquivo(nomeArquivo);
    if (arquivo.is_open()) {
        for (const auto& num : obj) {
            arquivo << num << " ";
        }
        arquivo.close();
    } else {
        cout << "Erro ao abrir o arquivo para escrita: " << nomeArquivo << endl;
    }
}

void ligador(string obj1, string obj2) {
    string conteudo1, conteudo2;

    if (!carregarArquivo(obj1, conteudo1) || !carregarArquivo(obj2, conteudo2)) {
        return;
    }

    processarDados(conteudo1, uso1, def1, co1, real1);
    processarDados(conteudo2, uso2, def2, co2, real2);

    novoObj.insert(novoObj.end(), co1.begin(), co1.end());
    fatorCorrecao = novoObj.size();
    novoObj.insert(novoObj.end(), co2.begin(), co2.end());
    
    defGlobal.clear();
    for (auto& par : def1) {
        defGlobal[par.first] = par.second;
    }
    for (auto& par : def2) {
        defGlobal[par.first] = par.second + fatorCorrecao;
    }

    resolveEnderecos();
    resolveReal();
    salvarNovoObj("prog1.e", novoObj);
    debug();
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cout << "Uso: " << argv[0] << " <obj1> <obj2>" << endl;
        return 1;
    }

    string obj1 = argv[1], obj2 = argv[2];
    ligador(obj1, obj2);
    return 0;
}
