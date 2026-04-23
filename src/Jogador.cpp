#include <iostream>
#include <random>
#include "Jogador.hpp"
#include "Jogo.hpp"
#include "Fase.hpp"

Jogador::Jogador( const std::string nome ) : nome( nome ), saldo(0), jogadaAtual(), apostaRodadaAtual(1)
{}

void Jogador::run (){
    std::cout << "Jogador " << nome << " esta jogando..." << std::endl;
    std::cout << "fase atual: " << jogo->getFaseAtual() << std::endl;
}

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
    if( valor >= 0 ){
        if( valor <= saldo ){ 
            saldo -= valor;
        } else{
            std::cerr << "Saldo insuficiente!!!" << std::endl;
        }
    } else{
        std::cerr << "Valor invalido!!!" << std::endl;
    }
}

void Jogador::realizarAposta( const int apostaMinima ){

    std::random_device rd; 
    std::mt19937 gen(rd()); 
    std::uniform_int_distribution<> distr( apostaMinima, getSaldo() + 10 ); 
    int apostaAleatoria = distr(gen);
    
    if( apostaAleatoria >= 0 ){
        if( apostaAleatoria <= saldo ){ 
            saldo -= apostaAleatoria;
            apostaRodadaAtual = apostaAleatoria;
        } else{ 
            apostaRodadaAtual = saldo; 
            saldo = 0; 
            std::cout << nome << " deu ALL-IN com " << apostaRodadaAtual << " fichas!" << std::endl;
        }
    } else{
        std::cerr << "Valor invalido!!!" << std::endl;
    }

}

void Jogador::resetarRodada(){

    apostaRodadaAtual = 1;
    ativoNaRodada = false;

}

void Jogador::setJogo( Jogo *jogo ){
    this->jogo = jogo;
}

void Jogador::coletarJogadaOculta(){

    std::random_device rd; 
    std::mt19937 gen(rd()); 
    std::uniform_int_distribution<> distr(1, 3); 
    Jogada jogadaAleatoria = (Jogada) distr(gen);

    if( this->isAtivoNaRodada( ) ){
        setJogadaAtual( jogadaAleatoria );
    }

}

