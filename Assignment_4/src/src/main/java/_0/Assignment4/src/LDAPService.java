package _0.Assignment4.src;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.ldap.core.AttributesMapper;
import org.springframework.ldap.core.LdapTemplate;
import org.springframework.ldap.support.LdapNameBuilder;
import org.springframework.stereotype.Service;

import javax.naming.Name;
import javax.naming.NamingException;
import java.util.List;

@Service
public class LDAPService {

    @Autowired
    private LdapTemplate ldapTemplate;

    private static final String BASE_DN = "ou=users,ou=system";

    public void addUser(LDAPUser ldapUser) {
        ldapTemplate.bind("uid=" + ldapUser.getUsername() + "," + BASE_DN, null, ldapUser.toAttributes());
    }


}
