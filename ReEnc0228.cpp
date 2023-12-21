#define PROFILE

#include "openfhe.h"

using namespace lbcrypto;

int main() {

    //指定乘法深度
    uint32_t multDepth = 6; 

    //指定縮放因子的bit長度
    uint32_t scaleModSize = 50;

    //指定密文中使用的明文槽數
    uint32_t batchSize = 4;

    //基於指定參數加密上下文
    CCParams<CryptoContextCKKSRNS> parameters;
    parameters.SetMultiplicativeDepth(multDepth);
    parameters.SetScalingModSize(scaleModSize);
    parameters.SetBatchSize(batchSize);

    CryptoContext<DCRTPoly> cc = GenCryptoContext(parameters);

    //啟用所需使用的功能特性
    cc->Enable(PKE);//
    cc->Enable(PRE);
    cc->Enable(KEYSWITCH);
    cc->Enable(LEVELEDSHE);
   
    std::cout << "CKKS scheme is using ring dimension " << cc->GetRingDimension() << std::endl << std::endl;

    //生成加密密鑰
    auto keys = cc->KeyGen();
    auto keys2 = cc->KeyGen();
    auto keys3 = cc->KeyGen();

    auto keysa = cc->KeyGen();//0228
    auto keysb = cc->KeyGen();//0228

    auto evalKey = cc->ReKeyGen(keys.secretKey,keys2.publicKey);//使用keys私鑰和keys2公鑰生成轉加密金鑰evalKey

    auto evalKeya = cc->ReKeyGen(keysa.secretKey,keys.publicKey);//0228
    auto evalKeyb = cc->ReKeyGen(keysb.secretKey,keys.publicKey);//0228

    auto evalKey2 = cc->ReKeyGen(keys2.secretKey,keysa.publicKey);//0228

    //Generate the digit size
    cc->EvalMultKeyGen(keys.secretKey);

    cc->EvalMultKeyGen(keysa.secretKey);//0228
    cc->EvalMultKeyGen(keysb.secretKey);//0228

    //Generate the rotation keys
    cc->EvalRotateKeyGen(keys.secretKey, {1, -2});

    // Encoding and encryption of inputs
    // Inputs
    std::vector<double> x1 = {1.1, 2.2, 3.3, 4.0};
    std::vector<double> x2 = {5.5, 4.4, 3.2, 4.0};

    std::vector<double> x3 = {2.0, 2.1, 2.2, 2.3};//0228
    std::vector<double> x4 = {3.0, 3.1, 3.2, 3.3};//0228

    //編碼為明文
    Plaintext ptxt1 = cc->MakeCKKSPackedPlaintext(x1);
    Plaintext ptxt2 = cc->MakeCKKSPackedPlaintext(x2);

    Plaintext ptxt3 = cc->MakeCKKSPackedPlaintext(x3);//0228
    Plaintext ptxt4 = cc->MakeCKKSPackedPlaintext(x4);//0228

    //輸出編碼後的明文
    std::cout << "Input x1: " << ptxt1 << std::endl;
    std::cout << "Input x2: " << ptxt2 << std::endl;

    std::cout << "Input x3: " << ptxt3 << std::endl;//0228 
    std::cout << "Input x4: " << ptxt4 << std::endl;//0228

    //加密編碼向量
    auto c1 = cc->Encrypt(keys.publicKey, ptxt1);
    auto c2 = cc->Encrypt(keys.publicKey, ptxt2);
        std::cout << "c1: " << c1 << std::endl;
    auto c3 = cc->Encrypt(keysa.publicKey, ptxt3);//0228
    auto c4 = cc->Encrypt(keysb.publicKey, ptxt4);//0228

    auto c12 = cc->ReEncrypt(c1, evalKey);//將c1向量用evalKey進行轉加密成c12向量 

    auto c32 = cc->ReEncrypt(c3, evalKeya);//0228
    auto c42 = cc->ReEncrypt(c4, evalKeyb);//0228  

    //Evaluation
    //密文相加
    auto cAdd = cc->EvalAdd(c1, c2);
   // std::cout << "Testing Add = " << cAdd;

    //密文相減
    auto cSub = cc->EvalSub(c1, c2);

    //明文與密文相乘
    auto cScalar = cc->EvalMult(c1, 4.0);

    //密文相乘
    auto cMul = cc->EvalMult(c1, c2);

    //密文旋轉
    auto cRot1 = cc->EvalRotate(c1, 1);
    auto cRot2 = cc->EvalRotate(c1, -2);

    //Decryption and output
    Plaintext result;
    Plaintext result2;
    Plaintext result3;//0228
    Plaintext result4;//0228
    // We set the cout precision to 8 decimal digits for a nicer output.
    // If you want to see the error/noise introduced by CKKS, bump it up
    // to 15 and it should become visible.
    std::cout.precision(8);

    std::cout << std::endl << "Results of homomorphic computations: " << std::endl;

    cc->Decrypt(keys.secretKey, c1, &result);
    result->SetLength(batchSize);
    std::cout << "x1 = " << result << std::endl;
    //std::cout << "\nEstimated precision in bits: " << result->GetLogPrecision() << std::endl;

    // Decrypt the result of addition
    cc->Decrypt(keys.secretKey, cAdd, &result);
    result->SetLength(batchSize);
    std::cout << "x1 + x2 = " << result << std::endl;
    //std::cout << "Estimated precision in bits: " << result->GetLogPrecision() << std::endl;

    // Decrypt the result of subtraction
    cc->Decrypt(keys.secretKey, cSub, &result);
    result->SetLength(batchSize);
    std::cout << "x1 - x2 = " << result << std::endl;

    // Decrypt the result of scalar multiplication
    cc->Decrypt(keys.secretKey, cScalar, &result);
    result->SetLength(batchSize);
    std::cout << " 4 * x1 = " << result << std::endl;

    // Decrypt the result of multiplication
    cc->Decrypt(keys.secretKey, cMul, &result);
    result->SetLength(batchSize);
    std::cout << "x1 * x2 = " << result << std::endl;

//------------------------------------------------------------//
    
    cc->Decrypt(keys2.secretKey, c12, &result2);//
    std::cout << "ReEncrypt_x1 = " << result2 << std::endl;//

    //將密文相加的結果cAdd轉加密成REcAdd
    auto REcAdd = cc->ReEncrypt(cAdd, evalKey);
    cc->Decrypt(keys2.secretKey, REcAdd, &result2);//將轉加密後的REcAdd使用keys2私鑰進行解密，若成功則代表解密權正確轉移
    result->SetLength(batchSize);
    std::cout << "ReEncrypt_x1 + x2 = " << result2 << std::endl;

    //將密文相減的結果cSub轉加密成REcSub
    auto REcSub = cc->ReEncrypt(cSub, evalKey);
    cc->Decrypt(keys2.secretKey, REcSub, &result2);//將轉加密後的REcSub使用keys2私鑰進行解密，若成功則代表解密權正確轉移
    result->SetLength(batchSize);
    std::cout << "ReEncrypt_x1 - x2 = " << result2 << std::endl;

    //將明文與密文相乘的結果cScalar轉加密成REcScalar
    auto REcScalar = cc->ReEncrypt(cScalar, evalKey);
    cc->Decrypt(keys2.secretKey, REcScalar, &result2);//將轉加密後的REcScalar使用keys2私鑰進行解密，若成功則代表解密權正確轉移
    result->SetLength(batchSize);
    std::cout << "ReEncrypt_4 * x1 = " << result2 << std::endl;

    //將密文相乘的結果cMul轉加密成REcMul
    auto REcMul = cc->ReEncrypt(cMul, evalKey);
    cc->Decrypt(keys2.secretKey, REcMul, &result2);//將轉加密後的REcMul使用keys2私鑰進行解密，若成功則代表解密權正確轉移
    result->SetLength(batchSize);
    std::cout << "ReEncrypt_x1 * x2 = " << result2 << std::endl;

//------------------------------------------------------------//
//0228--------------------------------------------------------//
 
    //將轉加密後的c3,c4相加，此時解密權為keys
    auto cAdd0228 = cc->EvalAdd(c32, c42);

    cc->Decrypt(keys.secretKey, cAdd0228, &result3);
    result->SetLength(batchSize);
    std::cout << "Rex3 + Rex4 = " << result3 << std::endl;

    //將轉加密後相加的結果cAdd0228再轉加密成REcAdd0228，若成功解密則代表解密權正確轉移至keys2
    auto REcAdd0228 = cc->ReEncrypt(cAdd0228, evalKey);
    cc->Decrypt(keys2.secretKey, REcAdd0228, &result3);
    result->SetLength(batchSize);
    std::cout << "ReReEncrypt_x3 + x4 = " << result3 << std::endl;

    //將轉加密後相加的結果REcAdd0228再轉加密成2REcAdd0228，若成功解密則代表解密權正確轉移至keysa
    auto RREcAdd0228 = cc->ReEncrypt(REcAdd0228, evalKey2);
    cc->Decrypt(keysa.secretKey, RREcAdd0228, &result4);
    result->SetLength(batchSize);
    std::cout << "2ReReEncrypt_x3 + x4 = " << result4 << std::endl;

//--------------------------------------------------------0228//

    // Decrypt the result of rotations
/*
    cc->Decrypt(keys.secretKey, cRot1, &result);
    result->SetLength(batchSize);
    std::cout << std::endl << "In rotations, very small outputs (~10^-10 here) correspond to 0's:" << std::endl;
    std::cout << "x1 rotate by 1 = " << result << std::endl;

    cc->Decrypt(keys.secretKey, cRot2, &result);
    result->SetLength(batchSize);
    std::cout << "x1 rotate by -2 = " << result << std::endl;
*/
    return 0;
}
