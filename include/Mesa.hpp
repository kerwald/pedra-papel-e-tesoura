#pragma once

class Mesa{
    private:
        int saldo;
    public:
        Mesa( const int saldoInicial );
        int getSaldo() const;
        void adicionarSaldo( const int valor );
        void reduzirSaldo( const int valor );
};