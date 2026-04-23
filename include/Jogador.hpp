#pragma once
#include <iostream>
#include <string>
#include "Jogada.hpp"
#include "Jogo.hpp"

class Jogador{
    private:
        Jogo *jogo;
        const std::string nome;
        int saldo;
        int apostaRodadaAtual;
        Jogada jogadaAtual;
        bool ativoNaRodada;
    public:
        Jogador( const std::string nome );
        void run (); 
        std::string getNome() const;
        int getSaldo() const;
        int getApostaRodadaAtual() const;
        Jogada getJogadaAtual() const;
        bool isAtivoNaRodada() const;
        void setAtivoNaRodada( const bool ativoNaRodada );
        void setJogadaAtual( const Jogada jogada );
        void adicionarSaldo( const int valor );
        void reduzirSaldo( const int valor );
        void realizarAposta( const int valor );
        void resetarRodada();
        void setJogo( Jogo *jogo );
};