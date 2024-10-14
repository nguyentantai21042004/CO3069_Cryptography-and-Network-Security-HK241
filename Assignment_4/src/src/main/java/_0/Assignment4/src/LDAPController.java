package _0.Assignment4.src;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class LDAPController {

    @Autowired
    private LDAPService ldapService;

    @GetMapping("/addUserForm")
    public String addUserForm(Model model) {
        model.addAttribute("ldapUser", new LDAPUser());
        return "addUser"; // Tên của template Thymeleaf cho form thêm user
    }
}
