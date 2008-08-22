#include "rsa.h"

#include <stdio.h>

RSA::RSA ()
{
        publicKeySet = false;
        privateKeySet = false;

        //init public key
        mpz_init2(m_e, 1024);
        mpz_init2(m_pubmod, 1024);
        //init private key
        mpz_init2(m_d, 1024);
        mpz_init2(m_prvmod, 1024);
}

RSA::~RSA ()
{
        //public key
        mpz_clear(m_e);
        mpz_clear(m_pubmod);
        //private key
        mpz_clear(m_d);
        mpz_clear(m_prvmod);
}

bool RSA::setPublicKey(const char* e, const char* m)
{
        int32_t res;
        res  = mpz_set_str(m_e, e, 10);
        res |= mpz_set_str(m_pubmod, m, 10);
        if (res == 0) {
                publicKeySet = true;
                return true;
        } else {
                return false;
        }
}

bool RSA::setPrivateKey(const char* d, const char* m)
{
        int32_t res;
        res  = mpz_set_str (m_d, d, 10);
        res |= mpz_set_str (m_prvmod, m, 10);
        if (res == 0) {
                privateKeySet = true;
                return true;
        } else {
                return false;
        }
}

bool RSA::encrypt(uint8_t* buffer, uint32_t size)
{
        if(publicKeySet) {
                mpz_t plain, crypt;
                mpz_init2 (plain, 1024);
                mpz_init2 (crypt, 1024);

                mpz_import (plain, 128, 1, 1, 0, 0, buffer);
                mpz_powm(crypt, plain, m_e, m_pubmod);
                
                //these 3 lines are taken directly from yact, i think i
                //can improve but its been a hassle to get this working
                //so for now it stays
                size_t count = (mpz_sizeinbase(crypt, 2) + 7)/8;
                memset(buffer, 0, size - count);
                mpz_export(&buffer[size - count], NULL, 1, 1, 0, 0, crypt);
                //mpz_export (buffer, NULL, 1, 1, 0, 0, crypt);

                mpz_clear(crypt);
                mpz_clear(plain);
                return true;
        } else {
                return false;
        }
}

bool RSA::decrypt(uint8_t* buffer, uint32_t size)
{
        if(privateKeySet) {
                mpz_t crypt, plain;
                mpz_init2 (plain, 1024);
                mpz_init2 (crypt, 1024);

                mpz_import(crypt, 128, 1, 1, 0, 0, buffer);

                mpz_powm (plain, crypt, m_d, m_prvmod);

                size_t count = (mpz_sizeinbase(plain, 2) + 7)/8;
                printf ("%d\n", count);
                memset(buffer, 0, size - count);
                mpz_export(&buffer[size - count], NULL, 1, 1, 0, 0, plain); 
                //mpz_export (buffer, NULL, 1, 1, 0, 0, plain);

                mpz_clear(crypt);
                mpz_clear(plain);
                return true;
        } else {
                return false;
        }
}

