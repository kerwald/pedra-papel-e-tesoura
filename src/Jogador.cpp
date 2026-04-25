#include <iostream>
#include <random>
#include "Jogada.hpp"
#include "Jogador.hpp"
#include "Jogo.hpp"
#include "Fase.hpp"

Jogador::Jogador( const std::string nome ) : jogo( nullptr ), nome( nome ), saldo(0), apostaRodadaAtual(1), jogadaAtual(), ativoNaRodada( false ) 
{
    //Pega o tempo do sistema em alta resolução (nanossegundos)
    auto time_seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    //Transforma o nome do jogador em um número inteiro único (hash)
    auto name_seed = std::hash<std::string>{}( nome );
    //Alimenta o motor aleatório misturando o tempo e o nome
    gen.seed( time_seed ^ name_seed );
}

void Jogador::run() {
    Fase ultimaFase = Fase::ESPERA;

    while (!jogo->isEncerrado()) {

        Fase fase = jogo->esperarProximaFase( ultimaFase );

        if (fase == Fase::ENCERRADO) break;

        if ( !ativoNaRodada ) {
            jogo->jogadorTerminouFase();
            ultimaFase = fase;
            continue;
        }

        switch (fase) {
            case Fase::APOSTA:
                realizarAposta();
                break;

            case Fase::AUMENTARAPOSTA:
                aumentarAposta();
                break;

            case Fase::JOGADA:
                coletarJogadaOculta();
                break;

            default:
                break;
        }

        ultimaFase = fase;
        jogo->jogadorTerminouFase();
    }
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

void Jogador::realizarAposta( ){

    std::uniform_int_distribution<> distr( 1, getSaldo() + (int) std::ceil( (double) ( getSaldo() ) / 2.0 ) ); 
    int apostaAleatoria = distr(gen);

    if( apostaAleatoria >= 0 ){
        if( apostaAleatoria <= saldo ){ 
            saldo -= apostaAleatoria;
            apostaRodadaAtual = apostaAleatoria;
            jogo->aumentarPote( apostaRodadaAtual );
        } else{ 
            apostaRodadaAtual = saldo; 
            saldo = 0; 
            jogo->aumentarPote( apostaRodadaAtual );
            jogo->imprimirMensagem( nome + " deu ALL-IN com " + std::to_string( apostaRodadaAtual ) + " fichas!" );
        }
    } else{
        jogo->imprimirMensagem( "Valor invalido!!!" );
    }

}

void Jogador::aumentarAposta(){

    std::uniform_int_distribution<> distr( 1, getSaldo() + (int) std::ceil( (double) ( getSaldo() ) / 2.0 ) ); 
    int apostaAleatoria = distr(gen);

    if( apostaAleatoria >= 0 ){
        if( apostaAleatoria <= saldo ){ 
            saldo -= apostaAleatoria;
            apostaRodadaAtual += apostaAleatoria;
            jogo->aumentarPote( apostaAleatoria );
        } else{ 
            apostaRodadaAtual += saldo; 
            jogo->aumentarPote( saldo );
            saldo = 0; 
            jogo->imprimirMensagem( nome + " deu ALL-IN com " + std::to_string( apostaRodadaAtual ) + " fichas!" );
        }
    } else{
        jogo->imprimirMensagem( "Valor invalido!!!" );
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

    std::uniform_int_distribution<> distr(0, 2); 
    Jogada jogadaAleatoria = (Jogada) distr(gen);

    if( this->isAtivoNaRodada( ) ){
        setJogadaAtual( jogadaAleatoria );
    }

}
