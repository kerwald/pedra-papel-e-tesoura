#include "Mesa.hpp"
#include <iostream>

Mesa::Mesa( const int saldoInicial ) : saldo( saldoInicial )
{}

int Mesa::getSaldo() const{
    return saldo;
}

void Mesa::adicionarSaldo( const int valor ){
    if( valor >=0 ){
        saldo += valor;
    } else{
        std::cerr << "valor invalido!!!" << std::endl;
    }
}

void Mesa::reduzirSaldo( const int valor ){
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