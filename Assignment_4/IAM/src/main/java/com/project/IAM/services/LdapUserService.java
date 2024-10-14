package com.project.IAM.services;

import com.project.IAM.models.LdapUser;
import lombok.RequiredArgsConstructor;
import org.springframework.ldap.core.LdapTemplate;
import org.springframework.stereotype.Service;

@Service
@RequiredArgsConstructor
public class LdapUserService {
    private final LdapTemplate ldapTemplate;

    public boolean addUser(LdapUser ldapUser){
        ldapTemplate.create(ldapUser);
        return true;
    }
}