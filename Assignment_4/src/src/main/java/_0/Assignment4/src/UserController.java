package _0.Assignment4.src;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.core.Authentication;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RestController;

@Controller
public class UserController {

    @Autowired
    private LDAPService ldapService;

    @GetMapping("/addUserForm")
    public String addUserForm( Model model) {
        model.addAttribute("ldapUser", new LDAPUser());
        return "addUser";
    }

    @PostMapping("/addUser")
    public String addUser( LDAPUser ldapUser) {
        ldapService.addUser(ldapUser);
        return "success";
    }

}
