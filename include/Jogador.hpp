#pragma once
#include <iostream>
#include <string>
#include "Jogada.hpp"

class Jogador{
    private:
        const std::string nome;
        int saldo;
        int apostaRodadaAtual;
        Jogada jogadaAtual;
        bool ativoNaRodada;
        bool temAllin;
    public:
        Jogador( const std::string nome );
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
};