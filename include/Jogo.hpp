#pragma once
#include <list>
#include "Jogador.hpp"
#include "Mesa.hpp"

class Jogo{
    private:
        std::list<Jogador> jogadores;
        Mesa mesa;
        bool  jogoEncerrado;
        const int LIMITE_EMPATES{ 5 };
    public:
        Jogo( std::list<Jogador> &jogadores );
        void iniciar( );
        void executarRodada( const int numeroRodada );
        void imprimirStatusMesa() const;
        int solicitarAposta( Jogador &j, const int apostaMinima );
        void coletarJogadasOcultas( const std::list<Jogador> &ativos );
        void limparConsole() const;
        int executarSubRodadaDesempate( std::list<Jogador> &ativos, const int pote );
        std::list<Jogador> determinarVencedores( std::list<Jogador> &ativos );
        void distribuirPremio( std::list<Jogador> ativos, std::list<Jogador> vencedores, const int pote );
        void aplicarPenalidadesLimiteEmpates( std::list<Jogador> ativos, const int pote );
        int contarJogadoresComSaldo() const;
        void declararCampeaoFinal()const;
};