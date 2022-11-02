// Copyright (c) 2022, Piotr Staniszewski

#include <iostream>

class Secret
{
public:
    void check() { std::cout << "Secret ID " << (m_secret_id == "secret_id" ? "OK" : "NOK") << std::endl; }

private:
    bool m_flag{false};
    double m_pi{3.14};
    const std::string m_secret_id{"secret_id"};
};

class Hack_secret
{
public:
    void hack_secret() { m_secret_id = "hacked_secret_id"; }

private:
    bool m_flag;
    double m_pi;
    std::string m_secret_id;
};

int main()
{
    std::cout << "hack const & private\n";

    Secret sec;
    sec.check();

    Hack_secret *hack_ptr = reinterpret_cast<Hack_secret *>(&sec);
    hack_ptr->hack_secret();

    sec.check();

    return EXIT_SUCCESS;
}
