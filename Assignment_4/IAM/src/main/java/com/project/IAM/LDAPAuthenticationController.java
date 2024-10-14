package com.project.IAM;

import com.project.IAM.services.LdapUserService;
import lombok.RequiredArgsConstructor;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

@RestController
@RequiredArgsConstructor
public class LDAPAuthenticationController {
    private final LdapUserService ldapService;

    @GetMapping("/")
    public String index() {
        return "Welcome to the home page!";
    }
}
