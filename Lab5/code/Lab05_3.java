import java.security.KeyPair;
import java.security.KeyPairGenerator;
import java.security.PrivateKey;
import java.security.PublicKey;
import java.security.Signature;
import javax.crypto.Cipher;
import javax.crypto.SecretKey;
import javax.crypto.KeyGenerator;
import java.util.Base64;

public class Lab05_3 {

    // Phương thức để ký trên một thông điệp
    public static byte[] sign(String message, PrivateKey privateKey) throws Exception {
        Signature signature = Signature.getInstance("SHA256withRSA");
        signature.initSign(privateKey);
        signature.update(message.getBytes());
        return signature.sign();
    }

    // Phương thức để xác minh chữ ký trên một thông điệp
    public static boolean verifySignature(String message, byte[] signatureBytes, PublicKey publicKey) throws Exception {
        Signature signature = Signature.getInstance("SHA256withRSA");
        signature.initVerify(publicKey);
        signature.update(message.getBytes());
        return signature.verify(signatureBytes);
    }

    // Phương thức để ký trên một thông điệp và sau đó mã hoá toàn bộ (dùng DES)
    public static byte[] signAndEncrypt(String message, PrivateKey privateKey, SecretKey desKey) throws Exception {
        // Ký thông điệp
        byte[] signatureBytes = sign(message, privateKey);

        // Kết hợp thông điệp và chữ ký
        byte[] messageBytes = message.getBytes();
        byte[] combined = new byte[messageBytes.length + signatureBytes.length];
        System.arraycopy(messageBytes, 0, combined, 0, messageBytes.length);
        System.arraycopy(signatureBytes, 0, combined, messageBytes.length, signatureBytes.length);

        // Mã hóa bằng DES
        Cipher desCipher = Cipher.getInstance("DES/ECB/PKCS5Padding");
        desCipher.init(Cipher.ENCRYPT_MODE, desKey);
        return desCipher.doFinal(combined);
    }

    // Phương thức để giải mã và xác minh chữ ký số
    public static boolean decryptAndVerify(byte[] encryptedMessage, PublicKey publicKey, SecretKey desKey)
            throws Exception {
        // Giải mã bằng DES
        Cipher desCipher = Cipher.getInstance("DES/ECB/PKCS5Padding");
        desCipher.init(Cipher.DECRYPT_MODE, desKey);
        byte[] decrypted = desCipher.doFinal(encryptedMessage);

        // Tách thông điệp và chữ ký
        int messageLength = decrypted.length - 256; // 256 bytes cho chữ ký RSA
        byte[] messageBytes = new byte[messageLength];
        byte[] signatureBytes = new byte[256];
        System.arraycopy(decrypted, 0, messageBytes, 0, messageLength);
        System.arraycopy(decrypted, messageLength, signatureBytes, 0, 256);

        // Xác minh chữ ký
        String message = new String(messageBytes);
        return verifySignature(message, signatureBytes, publicKey);
    }

    public static void main(String[] args) {
        try {
            // Tạo cặp khóa RSA
            KeyPairGenerator keyGen = KeyPairGenerator.getInstance("RSA");
            keyGen.initialize(2048);
            KeyPair keyPair = keyGen.generateKeyPair();
            PrivateKey privateKey = keyPair.getPrivate();
            PublicKey publicKey = keyPair.getPublic();

            // Tạo khóa DES
            KeyGenerator desKeyGen = KeyGenerator.getInstance("DES");
            desKeyGen.init(56); // DES sử dụng khóa 56 bit
            SecretKey desKey = desKeyGen.generateKey();

            String message = "Thông điệp để ký và mã hóa";

            // Thực hiện ký và mã hóa
            byte[] encryptedMessage = signAndEncrypt(message, privateKey, desKey);
            System.out.println(
                    "Thông điệp đã được ký và mã hóa: " + Base64.getEncoder().encodeToString(encryptedMessage));

            // Giải mã và xác minh chữ ký
            boolean isVerified = decryptAndVerify(encryptedMessage, publicKey, desKey);
            System.out.println("Xác minh chữ ký: " + (isVerified ? "Thành công" : "Thất bại"));

        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
