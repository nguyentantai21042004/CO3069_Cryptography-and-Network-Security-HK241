package com.project.IAM.controller;

import com.project.IAM.models.LdapUser;
import com.project.IAM.services.LdapUserService;
import lombok.RequiredArgsConstructor;
import org.springframework.web.bind.annotation.*;

@RestController
@RequiredArgsConstructor
@RequestMapping("/users")
public class UserController {
    private final LdapUserService ldapUserService;

    @PostMapping("/add")
    public String addUser(@RequestBody LdapUser ldapUser){
        if(!ldapUserService.addUser(ldapUser))
            return "Something wrongs";
        return "User added!";
    }
}
