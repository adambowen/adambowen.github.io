#include <string>

/*!
    \brief Get the public key used to verify server integrity

    \param[out] out On return, populated with the key data
*/
void get_public_key(std::string& out)
{
    out =
      "-----BEGIN PUBLIC KEY-----\n"
      "MIGeMA0GCSqGSIb3DQEBAQUAA4GMADCBiAKBgG9kLMEJN/PEtr8l3R\n"
      "ywUNlR8f3JUzSrg2SgSbAZFqeJOgf9OACP8trPy33y4vovyJMrQRse\n"
      "ojQMU7yhr3CqZgVGd8FkAzddxtlhumueXJjNPotfR4+2yCy11mQYnA\n"
      "Q7VFmjCyct9841zrd9PsyACxL1rqURdtYS5xjT4+SZm/U1AgMBAAE=\n"
      "-----END PUBLIC KEY-----\n";
}
