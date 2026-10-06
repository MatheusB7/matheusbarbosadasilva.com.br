#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>
#include <cctype>
#include <cstdlib>
#include <limits>

using namespace std;

const int JORNADA = 8 * 60;  // jornada diária em minutos

struct Dia {
    string data;          // DD/MM/AAAA
    vector<int> batidas;  // minutos desde 00:00, em ordem

    // Soma os pares entrada/saída do dia
    int trabalhado() const {
        int soma = 0;
        for (size_t i = 0; i + 1 < batidas.size(); i += 2)
            soma += batidas[i + 1] - batidas[i];
        return soma;
    }

    bool incompleto() const { return batidas.size() % 2 != 0; }
};

string formatarHora(int minutos) {
    ostringstream saida;
    saida << setfill('0') << setw(2) << minutos / 60 << ':' << setw(2) << minutos % 60;
    return saida.str();
}

string formatarSaldo(int minutos) {
    return (minutos < 0 ? "-" : "+") + formatarHora(abs(minutos));
}

bool lerHora(const string& texto, int& minutos) {
    int h, m;
    char separador;
    istringstream entrada(texto);
    if (!(entrada >> h >> separador >> m) || separador != ':' || h < 0 || h > 23 || m < 0 || m > 59)
        return false;
    minutos = h * 60 + m;
    return true;
}

bool dataValida(const string& data) {
    if (data.size() != 10 || data[2] != '/' || data[5] != '/') return false;
    for (int i = 0; i < 10; i++)
        if (i != 2 && i != 5 && !isdigit((unsigned char)data[i])) return false;
    int dia = stoi(data.substr(0, 2));
    int mes = stoi(data.substr(3, 2));
    return dia >= 1 && dia <= 31 && mes >= 1 && mes <= 12;
}

void horarioAtual(string& data, int& minutos) {
    time_t agora = time(nullptr);
    tm* t = localtime(&agora);
    char texto[11];
    strftime(texto, sizeof texto, "%d/%m/%Y", t);
    data = texto;
    minutos = t->tm_hour * 60 + t->tm_min;
}

class Funcionario {
public:
    Funcionario(int matricula, const string& nome) : matricula(matricula), nome(nome) {}

    int getMatricula() const { return matricula; }
    const string& getNome() const { return nome; }
    const vector<Dia>& getDias() const { return dias; }

    // Retorna false se a batida não for depois da última do mesmo dia
    bool baterPonto(const string& data, int minutos) {
        for (Dia& d : dias) {
            if (d.data == data) {
                if (minutos <= d.batidas.back()) return false;
                d.batidas.push_back(minutos);
                return true;
            }
        }
        dias.push_back({data, {minutos}});
        return true;
    }

private:
    int matricula;
    string nome;
    vector<Dia> dias;
};

class RelogioPonto {
public:
    bool cadastrar(int matricula, const string& nome) {
        if (buscar(matricula)) return false;
        funcionarios.emplace_back(matricula, nome);
        return true;
    }

    Funcionario* buscar(int matricula) {
        for (Funcionario& f : funcionarios)
            if (f.getMatricula() == matricula) return &f;
        return nullptr;
    }

    void espelho(const Funcionario& f) const {
        cout << "\nEspelho de ponto - " << f.getNome() << " (matrícula " << f.getMatricula() << ")\n";
        cout << "------------------------------------------------------------\n";
        int bancoDeHoras = 0;
        for (const Dia& d : f.getDias()) {
            cout << d.data << "  ";
            for (int b : d.batidas) cout << formatarHora(b) << ' ';
            if (d.incompleto()) {
                cout << " -> falta a batida de saída\n";
                continue;
            }
            int saldo = d.trabalhado() - JORNADA;
            bancoDeHoras += saldo;
            cout << " -> trabalhado " << formatarHora(d.trabalhado())
                 << ", saldo " << formatarSaldo(saldo) << '\n';
        }
        cout << "------------------------------------------------------------\n";
        cout << "Banco de horas: " << formatarSaldo(bancoDeHoras) << '\n';
    }

    void espelhoGeral() const {
        if (funcionarios.empty()) {
            cout << "Nenhum funcionário cadastrado.\n";
            return;
        }
        for (const Funcionario& f : funcionarios) espelho(f);
    }

    // Usa ponto e vírgula para abrir direto no Excel em português
    bool exportarCSV(const string& arquivo) const {
        ofstream csv(arquivo);
        if (!csv) return false;
        csv << "matricula;nome;data;batidas;trabalhado;saldo\n";
        for (const Funcionario& f : funcionarios) {
            for (const Dia& d : f.getDias()) {
                csv << f.getMatricula() << ';' << f.getNome() << ';' << d.data << ';';
                for (size_t i = 0; i < d.batidas.size(); i++)
                    csv << (i ? " " : "") << formatarHora(d.batidas[i]);
                if (d.incompleto())
                    csv << ";;incompleto\n";
                else
                    csv << ';' << formatarHora(d.trabalhado()) << ';'
                        << formatarSaldo(d.trabalhado() - JORNADA) << '\n';
            }
        }
        return true;
    }

private:
    vector<Funcionario> funcionarios;
};

int lerInteiro(const string& mensagem) {
    int valor;
    cout << mensagem;
    while (!(cin >> valor)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Digite um número: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return valor;
}

int main() {
    system("chcp 65001 > nul");

    RelogioPonto relogio;
    int opcao;

    do {
        cout << "\n***************************************************\n";
        cout << "                 RELÓGIO DE PONTO\n";
        cout << "         Criador: Matheus Barbosa da Silva\n";
        cout << "***************************************************\n\n";
        cout << "1 - Cadastrar funcionário\n";
        cout << "2 - Bater ponto agora\n";
        cout << "3 - Lançar batida manual\n";
        cout << "4 - Espelho de ponto\n";
        cout << "5 - Exportar CSV\n";
        cout << "0 - Sair\n\n";
        opcao = lerInteiro("Opção: ");

        if (opcao == 1) {
            int matricula = lerInteiro("Matrícula: ");
            string nome;
            cout << "Nome: ";
            getline(cin, nome);
            cout << (relogio.cadastrar(matricula, nome) ? "Funcionário cadastrado.\n" : "Essa matrícula já existe.\n");
        } else if (opcao == 4) {
            int matricula = lerInteiro("Matrícula (0 para todos): ");
            if (matricula == 0)
                relogio.espelhoGeral();
            else if (Funcionario* f = relogio.buscar(matricula))
                relogio.espelho(*f);
            else
                cout << "Funcionário não encontrado.\n";
        } else if (opcao == 2 || opcao == 3) {
            Funcionario* f = relogio.buscar(lerInteiro("Matrícula: "));
            if (!f) {
                cout << "Funcionário não encontrado.\n";
                continue;
            }

            string data, hora;
            int minutos;
            if (opcao == 2) {
                horarioAtual(data, minutos);
            } else {
                cout << "Data (DD/MM/AAAA): ";
                getline(cin, data);
                cout << "Hora (HH:MM): ";
                getline(cin, hora);
                if (!dataValida(data) || !lerHora(hora, minutos)) {
                    cout << "Data ou hora inválida.\n";
                    continue;
                }
            }

            if (f->baterPonto(data, minutos))
                cout << "Batida registrada: " << data << ' ' << formatarHora(minutos) << '\n';
            else
                cout << "A batida precisa ser depois da última do mesmo dia.\n";
        } else if (opcao == 5) {
            cout << (relogio.exportarCSV("ponto.csv") ? "Arquivo ponto.csv gerado.\n" : "Não foi possível criar o arquivo.\n");
        } else if (opcao != 0) {
            cout << "Opção inválida.\n";
        }
    } while (opcao != 0);

    return 0;
}
