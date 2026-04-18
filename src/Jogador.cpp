#include "Jogador.hpp"

Jogador::Jogador( const std::string nome ) : nome( nome ), saldo(0), jogadaAtual(), apostaRodadaAtual(1), temAllin{ true }
{}

std::string Jogador::getNome() const{
    return nome;
}

int Jogador::getSaldo() const{
    return saldo;
}

int Jogador::getApostaRodadaAtual() const{
    return apostaRodadaAtual;
}

Jogada Jogador::getJogadaAtual() const{
    return jogadaAtual;
}

bool Jogador::isAtivoNaRodada() const{
    return ativoNaRodada;
}

void Jogador::setAtivoNaRodada( const bool ativoNaRodada ){
    this->ativoNaRodada = ativoNaRodada;
}

void Jogador::setJogadaAtual( const Jogada jogada ){
    jogadaAtual = jogada;
}

void Jogador::adicionarSaldo( const int valor ){
    if( valor >= 0 ){
        saldo += valor;
    } else{
        std::cerr << "Saldo invalido!!!" << std::endl;
    }
}

void Jogador::reduzirSaldo( const int valor ){
    if( valor >=0 ){
        if( valor>=saldo ){
            saldo -= valor;
        } else{
            std::cerr << "Saldo insuficiente!!!" << std::endl;
        }
        
    } else{
        std::cerr << "Valor invalido!!!" << std::endl;
    }
}

void Jogador::realizarAposta( const int valor ){

    if( valor >=0 ){
        if( valor>=saldo ){
            saldo -= valor;
            apostaRodadaAtual = valor;
        } else{
            if( temAllin ){
                temAllin = false;
            } else{
                setAtivoNaRodada( false );
            }
        }
        
    } else{
        std::cerr << "Valor invalido!!!" << std::endl;
    }

}

void Jogador::resetarRodada(){

    apostaRodadaAtual = 1;
    ativoNaRodada = false;

}