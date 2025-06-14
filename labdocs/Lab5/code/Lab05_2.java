import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.security.KeyFactory;
import java.security.PrivateKey;
import java.security.PublicKey;
import java.security.spec.PKCS8EncodedKeySpec;
import java.security.spec.X509EncodedKeySpec;
import java.util.Base64;
import java.util.Scanner;

import javax.crypto.Cipher;

public class Lab05_2 {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        try {
            System.out.print("Nhập tên tập tin cần mã hóa/giải mã: ");
            String inputFile = scanner.nextLine();

            System.out.print("Nhập tên tập tin chứa khóa (public/private): ");
            String keyFile = scanner.nextLine();

            System.out.print("Nhập chế độ (encrypt/decrypt): ");
            String mode = scanner.nextLine();

            // Kiểm tra và tạo thư mục outputex2 nếu chưa tồn tại
            File outputDir = new File("outputex2");
            if (!outputDir.exists()) {
                outputDir.mkdir();
            }

            if (mode.equalsIgnoreCase("encrypt")) {
                PublicKey publicKey = loadPublicKey(keyFile);

                long startTime = System.currentTimeMillis();
                encryptFile(publicKey, inputFile, "outputex2/output.enc");
                long endTime = System.currentTimeMillis();

                System.out.println("Thời gian mã hóa: " + (endTime - startTime) + " ms");
            } else if (mode.equalsIgnoreCase("decrypt")) {
                PrivateKey privateKey = loadPrivateKey(keyFile);

                long startTime = System.currentTimeMillis();
                decryptFile(privateKey, "outputex2/output.enc", "outputex2/output.dec");
                long endTime = System.currentTimeMillis();

                System.out.println("Thời gian giải mã: " + (endTime - startTime) + " ms");
            } else {
                System.out.println("Chế độ không hợp lệ. Vui lòng chọn 'encrypt' hoặc 'decrypt'.");
            }

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private static PublicKey loadPublicKey(String filename) throws Exception {
        // Đọc nội dung file PEM
        String pem = new String(Files.readAllBytes(Paths.get(filename)));

        // Loại bỏ phần header và footer của PEM
        pem = pem.replace("-----BEGIN PUBLIC KEY-----", "")
                .replace("-----END PUBLIC KEY-----", "")
                .replaceAll("\\s", ""); // Loại bỏ khoảng trắng và xuống dòng

        // Giải mã Base64 để lấy byte array của khóa
        byte[] keyBytes = Base64.getDecoder().decode(pem);

        // Tạo đối tượng PublicKey từ byte array
        X509EncodedKeySpec spec = new X509EncodedKeySpec(keyBytes);
        KeyFactory keyFactory = KeyFactory.getInstance("RSA");
        return keyFactory.generatePublic(spec);
    }

    private static PrivateKey loadPrivateKey(String filename) throws Exception {
        // Đọc nội dung file PEM
        String pem = new String(Files.readAllBytes(Paths.get(filename)));

        // Loại bỏ phần header và footer của PEM
        pem = pem.replace("-----BEGIN PRIVATE KEY-----", "")
                .replace("-----END PRIVATE KEY-----", "")
                .replaceAll("\\s", ""); // Loại bỏ khoảng trắng và xuống dòng

        // Giải mã Base64 để lấy byte array của khóa
        byte[] keyBytes = Base64.getDecoder().decode(pem);

        // Tạo đối tượng PrivateKey từ byte array
        PKCS8EncodedKeySpec spec = new PKCS8EncodedKeySpec(keyBytes);
        KeyFactory keyFactory = KeyFactory.getInstance("RSA");
        return keyFactory.generatePrivate(spec);
    }

    private static void encryptFile(PublicKey publicKey, String inputFile, String outputFile) throws Exception {
        Cipher cipher = Cipher.getInstance("RSA/ECB/PKCS1Padding");
        cipher.init(Cipher.ENCRYPT_MODE, publicKey);

        try (InputStream in = new FileInputStream(inputFile);
                OutputStream out = new FileOutputStream(outputFile)) {
            byte[] buffer = new byte[117]; // Kích thước tối đa cho RSA 1024-bit với PKCS1 padding
            int bytesRead;
            while ((bytesRead = in.read(buffer)) != -1) {
                byte[] output = cipher.update(buffer, 0, bytesRead);
                if (output != null)
                    out.write(output);
            }
            byte[] output = cipher.doFinal();
            if (output != null)
                out.write(output);
        }
    }

    private static void decryptFile(PrivateKey privateKey, String inputFile, String outputFile) throws Exception {
        Cipher cipher = Cipher.getInstance("RSA/ECB/PKCS1Padding");
        cipher.init(Cipher.DECRYPT_MODE, privateKey);

        try (InputStream in = new FileInputStream(inputFile);
                OutputStream out = new FileOutputStream(outputFile)) {
            byte[] buffer = new byte[128]; // Kích thước tối đa cho RSA 1024-bit khối đã mã hóa
            int bytesRead;
            while ((bytesRead = in.read(buffer)) != -1) {
                byte[] output = cipher.update(buffer, 0, bytesRead);
                if (output != null)
                    out.write(output);
            }
            byte[] output = cipher.doFinal();
            if (output != null)
                out.write(output);
        }
    }
}
