#include "Jogador.hpp"

Jogador::Jogador( const std::string nome ) : nome( nome ), jogadaAtual(), apostaRodadaAtual(1)
{}

void Jogador::realizarAposta( const int valor ){
    saldo -= valor;
    apostaRodadaAtual = valor;
}