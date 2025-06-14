import java.io.FileOutputStream;
import java.io.IOException;
import java.security.KeyPair;
import java.security.KeyPairGenerator;
import java.security.PrivateKey;
import java.security.PublicKey;
import java.util.Base64;

public class GenerateRSAKeys {
    public static void main(String[] args) {
        try {
            // Tạo cặp khóa RSA
            KeyPairGenerator keyGen = KeyPairGenerator.getInstance("RSA");
            keyGen.initialize(2048); // Độ dài khóa: 2048-bit
            KeyPair keyPair = keyGen.generateKeyPair();

            // Lấy public key và private key
            PublicKey publicKey = keyPair.getPublic();
            PrivateKey privateKey = keyPair.getPrivate();

            // Lưu public key vào file
            saveKeyToFile("public.key", "PUBLIC KEY", publicKey.getEncoded());

            // Lưu private key vào file
            saveKeyToFile("private.key", "PRIVATE KEY", privateKey.getEncoded());

            System.out.println("Đã tạo cặp khóa RSA thành công!");
            System.out.println("Public Key lưu tại: public.key");
            System.out.println("Private Key lưu tại: private.key");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    // Hàm lưu key vào file ở định dạng PEM
    private static void saveKeyToFile(String filename, String keyType, byte[] keyBytes) throws IOException {
        // Chuyển byte array của key thành Base64
        String base64Key = Base64.getEncoder().encodeToString(keyBytes);

        // Định dạng PEM
        String pemKey = "-----BEGIN " + keyType + "-----\n";
        pemKey += base64Key.replaceAll("(.{64})", "$1\n"); // Mỗi dòng tối đa 64 ký tự
        pemKey += "\n-----END " + keyType + "-----\n";

        // Ghi key vào file
        try (FileOutputStream fos = new FileOutputStream(filename)) {
            fos.write(pemKey.getBytes());
        }
    }
}
