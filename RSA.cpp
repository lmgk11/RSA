#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <math.h>
#include <algorithm>
#include <numeric>
#include <cstring>
#include <vector>
#include <stdlib.h>

struct pubKey {
    long long e;
    long long n;
};

struct privKey {
    long long d;
    long long n;
    long long Cn;
};

long long cipherN(long long p, long long q) {                                           
    return abs((p-1)*(q-1))/std::__gcd((p-1), (q-1));                                   
}

long long get_e(long long cipherN) {                                                    
    for (long long possibleE = 2; possibleE < cipherN; possibleE++) {
        if (std::__gcd(possibleE, cipherN) == 1) return possibleE;                      
    }

    std::cerr << "Error: Could not find e value";                                       
    exit(1);
}

long long get_d(long long e, long long cN) {                                           
    long long d = 2; 
    while (1) {
        if ((d*e)%cN == 1) return d;                
        d++;
    }

    std::cerr << "Error: Could not find d value" << std::endl << "Program exited with exit status 1" << std::endl;                                       
    exit(1);
}

long long getRandomPrime() {                                       
    int primes[] = {1009 , 1013 , 1019 , 1021 , 1031 , 1033 , 1039 , 1049 , 1051 , 1061 , 1063 , 1069 , 1087 , 1091 , 1093 , 1097 , 1103 , 1109 , 1117 , 1123 , 1129 , 1151 , 1153 , 1163 , 1171 , 1181 , 1187 , 1193 , 1201 , 1213 , 1217 , 1223 , 1229 , 1231 , 1237 , 1249 , 1259 , 1277 , 1279 , 1283 , 1289 , 1291 , 1297 , 1301 , 1303 , 1307 , 1319 , 1321 , 1327 , 1361 , 1367 , 1373 , 1381 , 1399 , 1409 , 1423 , 1427 , 1429 , 1433 , 1439 , 1447 , 1451 , 1453 , 1459 , 1471 , 1481 , 1483 , 1487 , 1489 , 1493 , 1499 , 1511 , 1523 , 1531 , 1543 , 1549 , 1553 , 1559 , 1567 , 1571 , 1579 , 1583 , 1597 , 1601 , 1607 , 1609 , 1613 , 1619 , 1621 , 1627 , 1637 , 1657 , 1663 , 1667 , 1669 , 1693 , 1697 , 1699 , 1709 , 1721 , 1723 , 1733 , 1741 , 1747 , 1753 , 1759 , 1777 , 1783 , 1787 , 1789 , 1801 , 1811 , 1823 , 1831 , 1847 , 1861 , 1867 , 1871 , 1873 , 1877 , 1879 , 1889 , 1901 , 1907 , 1913 , 1931 , 1933 , 1949 , 1951 , 1973 , 1979 , 1987 , 1993 , 1997 , 1999};
    
    std::srand(unsigned(time(0)));                                          
    return primes[std::rand() % 134];
    
    std::cerr << "Error: Random index out of bounds";                                   
    exit(1);
}

long long power(long long x, long long y, long long modulo) {                          
    long long res = 1;
    
    x = x % modulo;

    if (x == 0) return 0;

    while (y > 0) {
        if (y & 1) {
            res = (res * x) % modulo;
        }
        y = y >> 1;
        x = (x * x) % modulo;
    }

    return res;
}

std::vector<long long> encrypt(std::string FILENAME, long long e, long long n) {
    std::ifstream inData(FILENAME);

    if (!inData) {
        std::cerr << "Error: File could not be opened" << std::endl << "Program exited with exit status 1" << std::endl;
        exit(1);
    }

    std::string stringData((std::istreambuf_iterator<char>(inData)), std::istreambuf_iterator<char>());

    int LengthOfMessage = stringData.length();
    int i = 0;

    char* chardata = new char[LengthOfMessage + 1];
    long long* asciiData = new long long[LengthOfMessage + 1];
    std::vector<long long> returnValues;
    std::strcpy(chardata, stringData.c_str());

    while (i < LengthOfMessage) {
        *(asciiData + i) = *(chardata + i);
        *(asciiData + i) = (long long)std::pow(*(asciiData + i), e) % n;
        returnValues.push_back(*(asciiData + i));

        i++;
    }

    return returnValues;
}

std::vector<long long> decrypt(std::vector<long long> encrypted, long long d, long long n) {
    long long* decrypted = new long long[encrypted.size()];
    std::vector<long long> returnValues;

    for(int i = 0; i < encrypted.size(); i++) {
        *(decrypted + i) = encrypted[i];
        *(decrypted + i) = power(*(decrypted + i), d, n);
        returnValues.push_back(*(decrypted + i));
    }

    std::cout << std::endl << std::endl;

    return returnValues;
}

int main() {
    clock_t begin;
    
    long long p = getRandomPrime();
    long long q = getRandomPrime();

    while (p == q) {
        q = getRandomPrime();
    }
    
    pubKey Pub;
    privKey Priv;

    Pub.n = p * q;
    Priv.n = Pub.n;

    Priv.Cn = cipherN(p, q);

    Pub.e = get_e(Priv.Cn);
    Priv.d = get_d(Pub.e, Priv.Cn);

    std::string ED;
    std::cout << "Would you like to encrypt or decrypt? [E/D]: ";
    getline(std::cin, ED);
    std::cout << std::endl;

    if(ED == "E" || ED == "e") {


        std::string Filesrc;
        std::cout << "Enter the path to the file you want to enccrypt: ";
        getline(std::cin, Filesrc);
        std::cout << "Do you have a key for encryption? [Y/N] ";
        std::string yn;
        getline(std::cin, yn);
        if (yn == "y" || yn == "Y") {
            std::cout << "Enter the variable e: ";
            std::cin >> Pub.e;
            std::cout << std::endl << "Enter variable N: ";
            std::cin >> Pub.n;
            std::cout << std:: endl << std::endl;
        } else if(yn == "n"|| yn == "N") {
            std::cout << "Encrypting with generated keys: e = " << Pub.e << ", N = " << Pub.n << " , d = " << Priv.d << std::endl << std::endl;
        } else {
            std::cerr << "'" << yn << "' is not a viable argument" << std::endl << "program exited with exit status 1";
            return 1;
        }



        begin = clock();
        std::vector<long long> encryptedmessage = encrypt(Filesrc, Pub.e, Pub.n);

        std::ofstream EncryptedSrc("Encrypted" + Filesrc);

        for (auto i = encryptedmessage.begin(); i != encryptedmessage.end(); ++i) {
            EncryptedSrc << *i << " ";
        }
    
    
    } else if (ED == "D" || ED == "d") {
        std::string Filesrc;
        std::cout << "Enter the path to the file you want to decrypt: ";
        getline(std::cin, Filesrc);

        std::ifstream EncryptedMsg(Filesrc);

        if (!EncryptedMsg) {
            std::cerr << "Error: Encrypted file could not be opened" << std::endl << "Program exited with exit status 1" << std::endl;
            exit(1);
        }

        std::string kd, kn;

        std::cout << "Enter the private key for decryption, d = ";
        getline(std::cin, kd);
        std::cout << "enter the private key for decryption, N = ";
        getline(std::cin, kn);

        begin = clock();

        Priv.d = (long long) std::stoi(kd);
        Priv.n = (long long) std::stoi(kn);

        std::string eMsg((std::istreambuf_iterator<char>(EncryptedMsg)), std::istreambuf_iterator<char>());

        std::string delimiter = " ";
        size_t pos = 0;
        std::string val;

        std::vector<long long> encrypted;

        while((pos = eMsg.find(delimiter)) != std::string::npos) {
            val = eMsg.substr(0, pos);
            encrypted.push_back((long long) std::stoi(val));
            eMsg.erase(0, pos + delimiter.length());
        }

        std::vector<long long> OrgMsg = decrypt(encrypted, Priv.d, Priv.n);

        for (auto i = OrgMsg.begin(); i != OrgMsg.end(); i++) {
            std::cout << (char) *i;
        }
        
        } else {
            std::cerr << ED << " is not a viable argument" << std::endl << "Program exited with exit status 1" << std::endl;
            return(1);
        }

    std::cout << std::endl;

    clock_t end = clock();
    if (ED == "e" || ED == "E") { std::cout << "Total time for encryption: " << (double) (end-begin) / CLOCKS_PER_SEC << "s" << std::endl; }
    if (ED == "d" || ED == "D") { std::cout << "Total time for decryption: " << (double) (end-begin) / CLOCKS_PER_SEC << "s" << std::endl; }

    system("Pause");

    return 0;
}