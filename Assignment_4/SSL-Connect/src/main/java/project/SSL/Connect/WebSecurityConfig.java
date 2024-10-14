package project.SSL.Connect;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.security.config.Customizer;
import org.springframework.security.config.annotation.web.builders.HttpSecurity;
import org.springframework.security.crypto.bcrypt.BCryptPasswordEncoder;
import org.springframework.security.web.SecurityFilterChain;
import org.springframework.security.ldap.userdetails.LdapAuthoritiesPopulator;
import org.springframework.security.ldap.DefaultSpringSecurityContextSource;
import org.springframework.security.ldap.userdetails.LdapUserDetailsService;
import org.springframework.security.ldap.search.FilterBasedLdapUserSearch;
import org.springframework.security.ldap.userdetails.DefaultLdapAuthoritiesPopulator;

import java.util.Arrays;

@Configuration
public class WebSecurityConfig {

    // Cấu hình luồng bảo mật chính
    @Bean
    public SecurityFilterChain securityFilterChain(HttpSecurity http) throws Exception {
        http
                .authorizeHttpRequests((authorize) -> authorize
                        .anyRequest().authenticated() // Yêu cầu xác thực cho mọi yêu cầu
                )
                .formLogin(Customizer.withDefaults()); // Sử dụng form login mặc định của Spring Security

        return http.build();
    }

    // Cấu hình nguồn dữ liệu LDAP
    @Bean
    public DefaultSpringSecurityContextSource contextSource() {
        return new DefaultSpringSecurityContextSource(
                Arrays.asList("ldap://localhost:8389/"), "dc=springframework,dc=org");
    }

    // Cấu hình LdapUserDetailsService với LdapUserSearch và LdapAuthoritiesPopulator
    @Bean
    public LdapUserDetailsService ldapUserDetailsService() {
        // Tìm kiếm người dùng dựa trên mẫu userDnPatterns
        FilterBasedLdapUserSearch userSearch = new FilterBasedLdapUserSearch("ou=people,dc=springframework,dc=org", "(uid={0})", contextSource());

        // Khai báo LdapAuthoritiesPopulator để ánh xạ người dùng với nhóm trong LDAP
        LdapAuthoritiesPopulator authoritiesPopulator = new DefaultLdapAuthoritiesPopulator(contextSource(), "ou=groups");

        // Trả về dịch vụ chi tiết người dùng LDAP
        return new LdapUserDetailsService(userSearch, authoritiesPopulator);
    }

    // Mã hóa mật khẩu với BCrypt
    @Bean
    public BCryptPasswordEncoder passwordEncoder() {
        return new BCryptPasswordEncoder();
    }
}
