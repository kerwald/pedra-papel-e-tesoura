#pragma once
#include <vector>
#include "Jogador.hpp"
#include "Mesa.hpp"

class Jogo{
    private:
        std::vector<Jogador> &jogadores;
        Mesa mesa;
        bool  jogoEncerrado;
        int empate;
        const int LIMITE_EMPATES{ 5 };
    public:
        Jogo( std::vector<Jogador> &jogadores );
        void iniciar( );
        void executarRodada( const int numeroRodada );
        void imprimirStatusMesa() const;
        int solicitarAposta( Jogador &j, const int apostaMinima );
        void coletarJogadasOcultas( const std::vector<Jogador*> &ativos );
        void limparConsole() const;
        int executarSubRodadaDesempate( std::vector<Jogador*> &ativos, const int pote );
        std::vector<Jogador*> determinarVencedores( std::vector<Jogador*> &ativos );
        void distribuirPremio( std::vector<Jogador*> &ativos, std::vector<Jogador*> &vencedores, int &pote );
        void aplicarPenalidadesLimiteEmpates( std::vector<Jogador*> &ativos, const int pote );
        int contarJogadoresComSaldo() const;
        void declararCampeaoFinal();
};