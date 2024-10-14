package com.project.IAM;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.ldap.core.LdapTemplate;
import org.springframework.ldap.core.support.BaseLdapPathContextSource;
import org.springframework.ldap.core.support.LdapContextSource;
import org.springframework.security.authentication.AuthenticationManager;
import org.springframework.security.config.Customizer;
import org.springframework.security.config.annotation.web.builders.HttpSecurity;
import org.springframework.security.config.annotation.web.configuration.EnableWebSecurity;
import org.springframework.security.config.ldap.LdapBindAuthenticationManagerFactory;
import org.springframework.security.web.SecurityFilterChain;

import java.nio.file.Paths;

@Configuration
@EnableWebSecurity
public class SecurityConfig {

    @Bean
    public SecurityFilterChain securityFilterChain(HttpSecurity httpSecurity) throws Exception {
        httpSecurity
                .authorizeHttpRequests((authorize) -> authorize
                        .anyRequest().authenticated()
                )
                .formLogin(Customizer.withDefaults());

        return httpSecurity.build();
    }

    @Bean
    public LdapTemplate ldapTemplate() {
        return new LdapTemplate(contextSource());
    }

    @Bean
    public LdapContextSource contextSource() {
        LdapContextSource ldapContextSource = new LdapContextSource();
        ldapContextSource.setUrl("ldaps://localhost:10636");  // Sử dụng ldaps cho SSL
        ldapContextSource.setBase("dc=example,dc=com");       // Cấu hình base DN
        ldapContextSource.setUserDn("uid=admin,ou=system");   // Tài khoản để xác thực
        ldapContextSource.setPassword("secret");              // Mật khẩu để xác thực

        System.setProperty("javax.net.ssl.trustStore", "D:/Workspaces/CO3069_Cryptography-and-Network-Security/Assignment_4/IAM/src/main/resources/truststore.jks");
        System.setProperty("javax.net.ssl.trustStorePassword", "truststorepassword");

        System.setProperty("javax.net.ssl.keyStore", "D:/Workspaces/CO3069_Cryptography-and-Network-Security/Assignment_4/IAM/src/main/resources/apacheds.jks");
        System.setProperty("javax.net.ssl.keyStorePassword", "keystorepassword");

        return ldapContextSource;
    }

    @Bean
    public AuthenticationManager authenticationManager(BaseLdapPathContextSource source) {
        LdapBindAuthenticationManagerFactory factory = new LdapBindAuthenticationManagerFactory(source);
        factory.setUserDnPatterns("cn={0},ou=users,ou=system");
        return factory.createAuthenticationManager();
    }
}
