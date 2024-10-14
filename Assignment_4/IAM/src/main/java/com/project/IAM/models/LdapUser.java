package com.project.IAM.models;

import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.RequiredArgsConstructor;
import lombok.Setter;

@NoArgsConstructor
@Getter
@Setter
public class LdapUser {
    private String cn;

    private String sn;

    private String password;

    private String username;

    private String faceEmbedding;  // Lưu chuỗi base64 của embedding khuôn mặt
}