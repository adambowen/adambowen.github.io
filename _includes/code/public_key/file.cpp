#include <string>
#include <fstream>
#include <sstream>

enum public_key_error_type
{
    no_error,
    error_opening_file,
    error_reading_data,
    invalid_key
};

std::string get_public_key_filename();

/*!
    \brief Get the public key used to verify server integrity

    \param[out] out On return, populated with the key data
*/
public_key_error_type get_public_key(std::string& out)
{
    std::ifstream in;
    in.open(get_public_key_filename());
    if (!in.is_open())
    {
        return error_opening_file;
    }

    std::stringstream buffer;
    if (!(buffer << in.rdbuf()))
    {
        return error_reading_data;
    }

    const std::string header =  "-----BEGIN PUBLIC KEY-----\n";
    const std::string footer =  "-----END PUBLIC KEY-----\n";

    std::string key = buffer.str();

    if (key.length() < header.length() + footer.length())
    {
        return invalid_key;
    }

    if (key.substr(0, header.length()) != header)
    {
        return invalid_key;
    }

    if (key.substr(
        key.length() - footer.length(),
        footer.length()) != footer)
    {
        return invalid_key;
    }

    out = key;
    return no_error;
}
