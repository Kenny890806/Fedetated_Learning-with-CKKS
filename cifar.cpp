#define PROFILE
#include <iterator>
#include <iostream>
#include <vector>
#include <array>
#include "openfhe.h"

#include "ciphertext-ser.h"
#include "cryptocontext-ser.h"
#include "key/key-ser.h"
#include "scheme/ckksrns/ckksrns-ser.h"

using namespace lbcrypto;
using namespace std;
const std::string DATAFOLDER = "tmp_dir"; //務必先於目的資料夾創建DATAFOLDER

extern "C" void keygenSer() {
    //指定乘法深度
    uint32_t multDepth = 16;//6

    //指定縮放因子的bit長度
    uint32_t scaleModSize = 50;//50

    //指定密文中使用的明文槽數
    //要動態調整
    uint32_t batchSize = 65536*2;//32768*128

    //基於指定參數加密上下文
    CCParams<CryptoContextCKKSRNS> parameters;
    parameters.SetMultiplicativeDepth(multDepth);
    parameters.SetScalingModSize(scaleModSize);
    //parameters.SetScalingModSize(40);
    //parameters.SetFirstModSize(50);
    parameters.SetBatchSize(batchSize);
    parameters.SetRingDim(131072*2);

    CryptoContext<DCRTPoly> cc = GenCryptoContext(parameters);

    //啟用所需使用的功能特性
    cc->Enable(PKE);//
    cc->Enable(PRE);
    cc->Enable(KEYSWITCH);
    cc->Enable(LEVELEDSHE);

    //序列化cryptocontext
    if (!Serial::SerializeToFile(DATAFOLDER + "/unique_cc.txt", cc, SerType::BINARY)) {
        cerr << "Error writing serialization of the crypto context to cc.txt" << endl;
    }
    cout << "The cryptocontext has been serialized." << endl;

    //生成金鑰
    //note that this keypair is unique keypair
    auto keyPair = cc->KeyGen();//KGC

    //序列化public key
    if (!Serial::SerializeToFile(DATAFOLDER + "/unique_key-public.txt", keyPair.publicKey, SerType::BINARY)) {
        cerr << "Error writing serialization of public key to key-public.txt" << endl;
    }
    cout << "The public key has been serialized." << endl;

    //序列化secret key
    if (!Serial::SerializeToFile(DATAFOLDER + "/unique_key-private.txt", keyPair.secretKey, SerType::BINARY)) {
        cerr << "Error writing serialization of private key to key-private.txt" << endl;
    }
    cout << "The secret key has been serialized." << endl;
}

extern "C" void uniqueEnc(double array[], const unsigned int size , const unsigned int client) {
    vector<double> temp(array, array+size);
    cout << "C++ Recevied Data:" << endl;
    //cout << temp << endl;
    
    //反序列化cryptocontext
    CryptoContext<DCRTPoly> cc;
    if (!Serial::DeserializeFromFile(DATAFOLDER + "/unique_cc.txt", cc, SerType::BINARY)) {
        cerr << "I cannot read serialization from " << DATAFOLDER + "/cc.txt" << endl;
    }
    //cout << "The cryptocontext has been deserialized." << endl;

    //反序列化public key
    PublicKey<DCRTPoly> pk;
    if (!Serial::DeserializeFromFile(DATAFOLDER + "/unique_key-public.txt", pk, SerType::BINARY)) {
        cerr << "Could not read public key" << endl;
    }
    //cout << "The public key has been deserialized." << endl;

    //反序列化private key
    PrivateKey<DCRTPoly> sk;
    if (!Serial::DeserializeFromFile(DATAFOLDER + "/unique_key-private.txt", sk, SerType::BINARY)) {
        cerr << "Could not read secret key" << endl;
    }
    //cout << "The secret key has been deserialized." << endl;
    Plaintext ptxt1 = cc->MakeCKKSPackedPlaintext(temp);

    //加密編碼向量
    auto c1 = cc->Encrypt(pk, ptxt1);
    string tempclient(to_string(client));
    //TODO:For loop for serialize all client parameter
    //Generate all client parameter ciphertext and serialize to ciphertext1.txt to ciphertext10.txt (assume that we have 10 clients)
    
    //序列化ciphertex
    if (!Serial::SerializeToFile(DATAFOLDER + "/ciphertext" + tempclient + ".txt", c1, SerType::BINARY)) { //DATAFOLDER + "/ciphertext" + i + ".txt"
        cerr << "Error writing serialization of ciphertext" + tempclient + " to ciphertext1.txt" << endl;    
    }
    cout << "The ciphertext" + tempclient + " has been serialized." << endl;

    //TODO:Test the Deserialize of ciphertext

    //cout << c1 << endl;
    //cout << "Note that After deserialized keys encrypted ciphertext" << endl;
    
    Plaintext result;
    cc->Decrypt(sk, c1, &result);
    //cout << result << endl;
    //cout << "Note that After deserialized keys decrypted ciphertext" << endl;
}

//HE.Add
extern "C" void ciphertextAdd() { //num of clients
    //TODO:Deserialize all clients ciphertext and add them toghter
    //指定乘法深度
 

    //啟用所需使用的功能特性
    
    Ciphertext<DCRTPoly> ct0;
    Ciphertext<DCRTPoly> ct1;
    Ciphertext<DCRTPoly> ct2;
    Ciphertext<DCRTPoly> ct3;
    Ciphertext<DCRTPoly> ct4;
    Ciphertext<DCRTPoly> ct5;
    Ciphertext<DCRTPoly> ct6;
    Ciphertext<DCRTPoly> ct7;
    Ciphertext<DCRTPoly> ct8;
    Ciphertext<DCRTPoly> ct9;
    /*
    int client = 9;

    for(int i = 0 ; i <= client ; i++)
    { 
        string tempi(to_string(i));
        
        if (Serial::DeserializeFromFile(DATAFOLDER + "/ciphertext" + tempi + ".txt", ct+tempi, SerType::BINARY) == false) {
            std::cerr << "Could not read the ciphertext" << std::endl;
            return 1;
        }
        std::cout << "The first ciphertext has been deserialized." << std::endl;
    }
    */
    if (Serial::DeserializeFromFile(DATAFOLDER + "/ciphertext0.txt", ct0, SerType::BINARY) == false) {
        std::cerr << "Could not read the ciphertext" << std::endl;
    }
    std::cout << "The ciphertext 0 has been deserialized." << std::endl;

    if (Serial::DeserializeFromFile(DATAFOLDER + "/ciphertext1.txt", ct1, SerType::BINARY) == false) {
        std::cerr << "Could not read the ciphertext" << std::endl;
    }
    std::cout << "The ciphertext 1 has been deserialized." << std::endl;

    if (Serial::DeserializeFromFile(DATAFOLDER + "/ciphertext2.txt", ct2, SerType::BINARY) == false) {
        std::cerr << "Could not read the ciphertext" << std::endl;
    
    }
    std::cout << "The ciphertext 2 has been deserialized." << std::endl;

    if (Serial::DeserializeFromFile(DATAFOLDER + "/ciphertext3.txt", ct3, SerType::BINARY) == false) {
        std::cerr << "Could not read the ciphertext" << std::endl;
    
    }
    std::cout << "The ciphertext 3 has been deserialized." << std::endl;

    if (Serial::DeserializeFromFile(DATAFOLDER + "/ciphertext4.txt", ct4, SerType::BINARY) == false) {
        std::cerr << "Could not read the ciphertext" << std::endl;
    
    }
    std::cout << "The ciphertext 4 has been deserialized." << std::endl;

    if (Serial::DeserializeFromFile(DATAFOLDER + "/ciphertext5.txt", ct5, SerType::BINARY) == false) {
        std::cerr << "Could not read the ciphertext" << std::endl;
    
    }
    std::cout << "The ciphertext 5 has been deserialized." << std::endl;

    if (Serial::DeserializeFromFile(DATAFOLDER + "/ciphertext6.txt", ct6, SerType::BINARY) == false) {
        std::cerr << "Could not read the ciphertext" << std::endl;
    
    }
    std::cout << "The ciphertext 6 has been deserialized." << std::endl;

    if (Serial::DeserializeFromFile(DATAFOLDER + "/ciphertext7.txt", ct7, SerType::BINARY) == false) {
        std::cerr << "Could not read the ciphertext" << std::endl;
    
    }
    std::cout << "The ciphertext 7 has been deserialized." << std::endl;

    if (Serial::DeserializeFromFile(DATAFOLDER + "/ciphertext8.txt", ct8, SerType::BINARY) == false) {
        std::cerr << "Could not read the ciphertext" << std::endl;
    
    }
    std::cout << "The ciphertext 8 has been deserialized." << std::endl;

    if (Serial::DeserializeFromFile(DATAFOLDER + "/ciphertext9.txt", ct9, SerType::BINARY) == false) {
        std::cerr << "Could not read the ciphertext" << std::endl;
     
    }
    std::cout << "The ciphertext 9 has been deserialized." << std::endl;

    CryptoContext<DCRTPoly> cc;
    if (!Serial::DeserializeFromFile(DATAFOLDER + "/unique_cc.txt", cc, SerType::BINARY)) {
        cerr << "I cannot read serialization from " << DATAFOLDER + "/cc.txt" << endl;
    }
    
    cc->Enable(PKE);//
    cc->Enable(PRE);
    cc->Enable(KEYSWITCH);
    cc->Enable(LEVELEDSHE);
    //同態加法
    auto cAdd0 = cc->EvalAdd(ct0, ct1);
    auto cAdd1 = cc->EvalAdd(cAdd0, ct2);
    auto cAdd2 = cc->EvalAdd(cAdd1, ct3);
    auto cAdd3 = cc->EvalAdd(cAdd2, ct4);
    auto cAdd4 = cc->EvalAdd(cAdd3, ct5);
    auto cAdd5 = cc->EvalAdd(cAdd4, ct6);
    auto cAdd6 = cc->EvalAdd(cAdd5, ct7);
    auto cAdd7 = cc->EvalAdd(cAdd6, ct8);
    auto cAdd = cc->EvalAdd(cAdd7, ct9);

    //cout << cAdd << endl;
    //序列化
    if (!Serial::SerializeToFile(DATAFOLDER + "/cAdd_ciphertext.txt", cAdd, SerType::BINARY)) { 
        cerr << "Error writing serialization of cAdd_ciphertext to cAdd_ciphertext.txt" << endl;
    }
    cout << "The cAdd_ciphertext.txt has been serialized." << endl;

    /*
    //-----------------------------------------------------------------------------------------------------------------------------
    //反序列化
    Ciphertext<DCRTPoly> testcAdd;
    if (Serial::DeserializeFromFile(DATAFOLDER + "/cAdd_ciphertext.txt", testcAdd, SerType::BINARY) == false) {
        std::cerr << "Could not read the ciphertext" << std::endl;

    }
    std::cout << "The cAdd_ciphertext has been deserialized." << std::endl;
    //反序列化private key
    PrivateKey<DCRTPoly> temp_sk;
    if (!Serial::DeserializeFromFile(DATAFOLDER + "/unique_key-private.txt", temp_sk, SerType::BINARY)) {
        cerr << "Could not read secret key" << endl;
    }
    Plaintext result;
    cc->Decrypt(temp_sk, testcAdd, &result);

    
    */
}



extern "C" void evalKeygen() {

    //反序列化cc
    CryptoContext<DCRTPoly> cc;
    if (!Serial::DeserializeFromFile(DATAFOLDER + "/unique_cc.txt", cc, SerType::BINARY)) {
        cerr << "I cannot read serialization from " << DATAFOLDER + "/cc.txt" << endl;
    }
    //cout << "The cryptocontext has been deserialized." << endl;

 

    //反序列化KGC private key
    PrivateKey<DCRTPoly> sk;
    if (!Serial::DeserializeFromFile(DATAFOLDER + "/unique_key-private.txt", sk, SerType::BINARY)) {
        cerr << "Could not read secret key" << endl;
    }
    cc->Enable(PKE);//
    cc->Enable(PRE);
    cc->Enable(KEYSWITCH);
    cc->Enable(LEVELEDSHE);

    //-------------------------------------------------------ClientKey_0 Gen-------------------------------------------------//
    auto clientKey_0 = cc->KeyGen();//client0
    //序列化public key
    if (!Serial::SerializeToFile(DATAFOLDER + "/clientKey_0-public.txt", clientKey_0.publicKey, SerType::BINARY)) {
        cerr << "Error writing serialization of public key to key-public.txt" << endl;
    }
    //cout << "The public key has been serialized." << endl;

    //序列化secret key
    if (!Serial::SerializeToFile(DATAFOLDER + "/clientKey_0-private.txt", clientKey_0.secretKey, SerType::BINARY)) {
        cerr << "Error writing serialization of private key to key-private.txt" << endl;
    }

    cout << "Client_0's Secret and Public keys have been serialized." << endl;

    //-------------------------------------------------------ClientKey_1 Gen-------------------------------------------------//
    auto clientKey_1 = cc->KeyGen();//client1
    //序列化public key
    if (!Serial::SerializeToFile(DATAFOLDER + "/clientKey_1-public.txt", clientKey_1.publicKey, SerType::BINARY)) {
        cerr << "Error writing serialization of public key to key-public.txt" << endl;
    }
    //cout << "The public key has been serialized." << endl;

    //序列化secret key
    if (!Serial::SerializeToFile(DATAFOLDER + "/clientKey_1-private.txt", clientKey_1.secretKey, SerType::BINARY)) {
        cerr << "Error writing serialization of private key to key-private.txt" << endl;
    }

    cout << "Client_1's Secret and Public keys have been serialized." << endl;

    //-------------------------------------------------------ClientKey_2 Gen-------------------------------------------------//
    auto clientKey_2 = cc->KeyGen();//client2
    //序列化public key
    if (!Serial::SerializeToFile(DATAFOLDER + "/clientKey_2-public.txt", clientKey_2.publicKey, SerType::BINARY)) {
        cerr << "Error writing serialization of public key to key-public.txt" << endl;
    }
    //cout << "The public key has been serialized." << endl;

    //序列化secret key
    if (!Serial::SerializeToFile(DATAFOLDER + "/clientKey_2-private.txt", clientKey_2.secretKey, SerType::BINARY)) {
        cerr << "Error writing serialization of private key to key-private.txt" << endl;
    }

    cout << "Client_2's Secret and Public keys have been serialized." << endl;

    //-------------------------------------------------------ClientKey_3 Gen-------------------------------------------------//
    auto clientKey_3 = cc->KeyGen();//client3
    //序列化public key
    if (!Serial::SerializeToFile(DATAFOLDER + "/clientKey_3-public.txt", clientKey_3.publicKey, SerType::BINARY)) {
        cerr << "Error writing serialization of public key to key-public.txt" << endl;
    }
    //cout << "The public key has been serialized." << endl;

    //序列化secret key
    if (!Serial::SerializeToFile(DATAFOLDER + "/clientKey_3-private.txt", clientKey_3.secretKey, SerType::BINARY)) {
        cerr << "Error writing serialization of private key to key-private.txt" << endl;
    }

    cout << "Client_3's Secret and Public keys have been serialized." << endl;

    //-------------------------------------------------------ClientKey_4 Gen-------------------------------------------------//
    auto clientKey_4 = cc->KeyGen();//client4
    //序列化public key
    if (!Serial::SerializeToFile(DATAFOLDER + "/clientKey_4-public.txt", clientKey_4.publicKey, SerType::BINARY)) {
        cerr << "Error writing serialization of public key to key-public.txt" << endl;
    }
    //cout << "The public key has been serialized." << endl;

    //序列化secret key
    if (!Serial::SerializeToFile(DATAFOLDER + "/clientKey_4-private.txt", clientKey_4.secretKey, SerType::BINARY)) {
        cerr << "Error writing serialization of private key to key-private.txt" << endl;
    }

    cout << "Client_4's Secret and Public keys have been serialized." << endl;

    //-------------------------------------------------------ClientKey_5 Gen-------------------------------------------------//
    auto clientKey_5 = cc->KeyGen();//client5
    //序列化public key
    if (!Serial::SerializeToFile(DATAFOLDER + "/clientKey_5-public.txt", clientKey_5.publicKey, SerType::BINARY)) {
        cerr << "Error writing serialization of public key to key-public.txt" << endl;
    }
    //cout << "The public key has been serialized." << endl;

    //序列化secret key
    if (!Serial::SerializeToFile(DATAFOLDER + "/clientKey_5-private.txt", clientKey_5.secretKey, SerType::BINARY)) {
        cerr << "Error writing serialization of private key to key-private.txt" << endl;
    }

    cout << "Client_5's Secret and Public keys have been serialized." << endl;

    //-------------------------------------------------------ClientKey_6 Gen-------------------------------------------------//
    auto clientKey_6 = cc->KeyGen();//client6
    //序列化public key
    if (!Serial::SerializeToFile(DATAFOLDER + "/clientKey_6-public.txt", clientKey_6.publicKey, SerType::BINARY)) {
        cerr << "Error writing serialization of public key to key-public.txt" << endl;
    }
    //cout << "The public key has been serialized." << endl;

    //序列化secret key
    if (!Serial::SerializeToFile(DATAFOLDER + "/clientKey_6-private.txt", clientKey_6.secretKey, SerType::BINARY)) {
        cerr << "Error writing serialization of private key to key-private.txt" << endl;
    }

    cout << "Client_6's Secret and Public keys have been serialized." << endl;

    //-------------------------------------------------------ClientKey_7 Gen-------------------------------------------------//
    auto clientKey_7 = cc->KeyGen();//client7
    //序列化public key
    if (!Serial::SerializeToFile(DATAFOLDER + "/clientKey_7-public.txt", clientKey_7.publicKey, SerType::BINARY)) {
        cerr << "Error writing serialization of public key to key-public.txt" << endl;
    }
    //cout << "The public key has been serialized." << endl;

    //序列化secret key
    if (!Serial::SerializeToFile(DATAFOLDER + "/clientKey_7-private.txt", clientKey_7.secretKey, SerType::BINARY)) {
        cerr << "Error writing serialization of private key to key-private.txt" << endl;
    }

    cout << "Client_7's Secret and Public keys have been serialized." << endl;

    //-------------------------------------------------------ClientKey_8 Gen-------------------------------------------------//
    auto clientKey_8 = cc->KeyGen();//client8
    //序列化public key
    if (!Serial::SerializeToFile(DATAFOLDER + "/clientKey_8-public.txt", clientKey_8.publicKey, SerType::BINARY)) {
        cerr << "Error writing serialization of public key to key-public.txt" << endl;
    }
    //cout << "The public key has been serialized." << endl;

    //序列化secret key
    if (!Serial::SerializeToFile(DATAFOLDER + "/clientKey_8-private.txt", clientKey_8.secretKey, SerType::BINARY)) {
        cerr << "Error writing serialization of private key to key-private.txt" << endl;
    }

    cout << "Client_8's Secret and Public keys have been serialized." << endl;
    //-------------------------------------------------------ClientKey_9 Gen-------------------------------------------------//
    auto clientKey_9 = cc->KeyGen();//client9
    //序列化public key
    if (!Serial::SerializeToFile(DATAFOLDER + "/clientKey_9-public.txt", clientKey_9.publicKey, SerType::BINARY)) {
        cerr << "Error writing serialization of public key to key-public.txt" << endl;
    }
    //cout << "The public key has been serialized." << endl;

    //序列化secret key
    if (!Serial::SerializeToFile(DATAFOLDER + "/clientKey_9-private.txt", clientKey_9.secretKey, SerType::BINARY)) {
        cerr << "Error writing serialization of private key to key-private.txt" << endl;
    }

    cout << "Client_9's Secret and Public keys have been serialized." << endl;

    //-------------------------------------------------------Generate reciphertext-------------------------------------------------//
    

    // 反序列化 cAdd_ciphertext
    Ciphertext<DCRTPoly> re_ct;
    if (Serial::DeserializeFromFile(DATAFOLDER + "/cAdd_ciphertext.txt", re_ct, SerType::BINARY) == false) {
        std::cerr << "Could not read the cAdd_ciphertext" << std::endl;
    }


    //------------------------------Client 0-----------------------------------//
    auto evalKey_0 = cc->ReKeyGen(sk, clientKey_0.publicKey);

    auto reCiphertext_0 = cc->ReEncrypt(re_ct, evalKey_0);
    if (!Serial::SerializeToFile(DATAFOLDER + "/reCiphertext_0.txt", reCiphertext_0, SerType::BINARY)) {
        cerr << "Error writing serialization of reCiphertext_0 to reCiphertext_0.txt" << endl;
    }
    std::cout << "The ciphertext 0 has been reEncrypt & serialized !." << std::endl;

    //------------------------------Client 1-----------------------------------//
    auto evalKey_1 = cc->ReKeyGen(sk, clientKey_1.publicKey);

    auto reCiphertext_1 = cc->ReEncrypt(re_ct, evalKey_1);
    if (!Serial::SerializeToFile(DATAFOLDER + "/reCiphertext_1.txt", reCiphertext_1, SerType::BINARY)) {
        cerr << "Error writing serialization of reCiphertext_1 to reCiphertext_1.txt" << endl;
    }
    std::cout << "The ciphertext 1 has been reEncrypt & serialized !." << std::endl;

    //------------------------------Client 2-----------------------------------//
    auto evalKey_2 = cc->ReKeyGen(sk, clientKey_2.publicKey);

    auto reCiphertext_2 = cc->ReEncrypt(re_ct, evalKey_2);
    if (!Serial::SerializeToFile(DATAFOLDER + "/reCiphertext_2.txt", reCiphertext_2, SerType::BINARY)) {
        cerr << "Error writing serialization of reCiphertext_2 to reCiphertext_2.txt" << endl;
    }
    std::cout << "The ciphertext 2 has been reEncrypt & serialized !." << std::endl;

    //------------------------------Client 3-----------------------------------//
    auto evalKey_3 = cc->ReKeyGen(sk, clientKey_3.publicKey);

    auto reCiphertext_3 = cc->ReEncrypt(re_ct, evalKey_3);
    if (!Serial::SerializeToFile(DATAFOLDER + "/reCiphertext_3.txt", reCiphertext_3, SerType::BINARY)) {
        cerr << "Error writing serialization of reCiphertext_3 to reCiphertext_3.txt" << endl;
    }
    std::cout << "The ciphertext 3 has been reEncrypt & serialized !." << std::endl;

    //------------------------------Client 4-----------------------------------//
    auto evalKey_4 = cc->ReKeyGen(sk, clientKey_4.publicKey);

    auto reCiphertext_4 = cc->ReEncrypt(re_ct, evalKey_4);
    if (!Serial::SerializeToFile(DATAFOLDER + "/reCiphertext_4.txt", reCiphertext_4, SerType::BINARY)) {
        cerr << "Error writing serialization of reCiphertext_4 to reCiphertext_4.txt" << endl;
    }
    std::cout << "The ciphertext 4 has been reEncrypt & serialized !." << std::endl;

    //------------------------------Client 5-----------------------------------//
    auto evalKey_5 = cc->ReKeyGen(sk, clientKey_5.publicKey);

    auto reCiphertext_5 = cc->ReEncrypt(re_ct, evalKey_5);
    if (!Serial::SerializeToFile(DATAFOLDER + "/reCiphertext_5.txt", reCiphertext_5, SerType::BINARY)) {
        cerr << "Error writing serialization of reCiphertext_5 to reCiphertext_5.txt" << endl;
    }
    std::cout << "The ciphertext 5 has been reEncrypt & serialized !." << std::endl;

    //------------------------------Client 6-----------------------------------//
    auto evalKey_6 = cc->ReKeyGen(sk, clientKey_6.publicKey);

    auto reCiphertext_6 = cc->ReEncrypt(re_ct, evalKey_6);
    if (!Serial::SerializeToFile(DATAFOLDER + "/reCiphertext_6.txt", reCiphertext_6, SerType::BINARY)) {
        cerr << "Error writing serialization of reCiphertext_6 to reCiphertext_6.txt" << endl;
    }
    std::cout << "The ciphertext 6 has been reEncrypt & serialized !." << std::endl;

    //------------------------------Client 7-----------------------------------//
    auto evalKey_7 = cc->ReKeyGen(sk, clientKey_7.publicKey);

    auto reCiphertext_7 = cc->ReEncrypt(re_ct, evalKey_7);
    if (!Serial::SerializeToFile(DATAFOLDER + "/reCiphertext_7.txt", reCiphertext_7 , SerType::BINARY)) {
        cerr << "Error writing serialization of reCiphertext_7 to reCiphertext_7.txt" << endl;
    }
    std::cout << "The ciphertext 7 has been reEncrypt & serialized !." << std::endl;

    //------------------------------Client 8-----------------------------------//
    auto evalKey_8 = cc->ReKeyGen(sk, clientKey_8.publicKey);

    auto reCiphertext_8 = cc->ReEncrypt(re_ct, evalKey_8);
    if (!Serial::SerializeToFile(DATAFOLDER + "/reCiphertext_8.txt", reCiphertext_8, SerType::BINARY)) {
        cerr << "Error writing serialization of reCiphertext_8 to reCiphertext_8.txt" << endl;
    }
    std::cout << "The ciphertext 8 has been reEncrypt & serialized !." << std::endl;

    //------------------------------Client 9-----------------------------------//
    auto evalKey_9 = cc->ReKeyGen(sk, clientKey_9.publicKey);

    auto reCiphertext_9 = cc->ReEncrypt(re_ct, evalKey_9);
    if (!Serial::SerializeToFile(DATAFOLDER + "/reCiphertext_9.txt", reCiphertext_9, SerType::BINARY)) {
        cerr << "Error writing serialization of reCiphertext_9 to reCiphertext_9.txt" << endl;
    }
    std::cout << "The ciphertext 9 has been reEncrypt & serialized !." << std::endl;










}

extern "C" double* redeEnc() {

    //uint32_t batchSize = 32768;

    //反序列化cc
    CryptoContext<DCRTPoly> cc;
    if (!Serial::DeserializeFromFile(DATAFOLDER + "/unique_cc.txt", cc, SerType::BINARY)) {
        cerr << "I cannot read serialization from " << DATAFOLDER + "/cc.txt" << endl;
    }
    //cout << "The cryptocontext has been deserialized." << endl;



    //反序列化client X's private key
    PrivateKey<DCRTPoly> client_0_sk;
    if (!Serial::DeserializeFromFile(DATAFOLDER + "/clientKey_0-private.txt", client_0_sk, SerType::BINARY)) {
        cerr << "Could not read secret key" << endl;
    }
    //cout << "The secret key has been deserialized." << endl;
   

    Ciphertext<DCRTPoly> reCiphertext;
    if (Serial::DeserializeFromFile(DATAFOLDER + "/reCiphertext_0.txt", reCiphertext, SerType::BINARY) == false) {
        std::cerr << "Could not read the ciphertext" << std::endl;
    }
    std::cout << "The ciphertext 0 has been deserialized." << std::endl;

    Plaintext result;

    cc->Decrypt(client_0_sk, reCiphertext, &result);
    //result-> SetLength(batchSize);
    //cout << result[0] << endl;
    /*
    double d_arr[result->GetRealPackedValue().size()];
    copy(result->GetRealPackedValue().begin(), result->GetRealPackedValue().end(), d_arr);
    
    cout << d_arr[0] << endl;
    cout << d_arr[21839] << endl;
    cout << d_arr[21840] << endl;
    cout << d_arr[21841] << endl;*/
    //vector<double> x ;
    //x.assign(result->GetRealPackedValue().begin(), result->GetRealPackedValue().end());
    //static double retarray[21840];
    double* retarray = new double[21840];

    cout << result->GetRealPackedValue().size() << endl;
    cout << result-> GetRealPackedValue()[0] << endl;
    cout << typeid(result->GetRealPackedValue()[0]).name() << endl;
    cout << result->GetRealPackedValue()[21839] << endl;
    cout << result->GetRealPackedValue()[30000] << endl;


    for (int i = 0; i <= 21839; i++) {
        retarray[i] = result->GetRealPackedValue()[i];
    }

    //cout << size(retarray) << endl;
    cout << retarray[0] << endl;
    /*
    cout << "0：" << x[0] << endl;
    cout << "21839：" << x[21839] << endl;
    cout << "30000：" << x[30000] << endl;
    cout << x.size() << endl;*/
    //cout << typeid(result).name() << endl;
    /*
    ofstream ofs;
    

    ofs.open("output.txt");
    if (!ofs.is_open()) {
        cout << "Failed to open file.\n";
        return 1; // EXIT_FAILURE
    }

    ofs << result ;
    
    ofs.close();
    */

    /*
    if (!Serial::SerializeToFile(DATAFOLDER + "/tempPlaintext.txt", result, SerType::BINARY)) {
        cerr << "Error writing serialization of result to tempPlaintext.txt" << endl;
    }
    std::cout << "The tempPlaintext has been serialized !." << std::endl;*/
    return retarray;
}

extern "C" double reDec() {
    return 0;
}