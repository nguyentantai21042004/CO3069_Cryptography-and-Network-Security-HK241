import java.io.*;
import java.util.Scanner;
import javax.crypto.*;
import javax.crypto.spec.*;

public class Lab05_1 {
	public static void main(String[] args) {
		@SuppressWarnings("resource")
		Scanner scanner = new Scanner(System.in);

		try {
			System.out.print("Nhập tên tập tin cần mã hóa/giải mã: ");
			String inputFile = scanner.nextLine();

			System.out.print("Nhập tên tập tin chứa khóa: ");
			String keyFile = scanner.nextLine();

			// Tạo khóa DES hoặc đọc từ tập tin
			SecretKey secretKey = generateOrReadKey(keyFile);

			// Tạo vector khởi tạo cho chế độ CBC
			byte[] iv = new byte[8];
			IvParameterSpec ivSpec = new IvParameterSpec(iv);

			String[] modes = { "DES/ECB/PKCS5Padding", "DES/ECB/NoPadding", "DES/CBC/PKCS5Padding",
					"DES/CBC/NoPadding" };

			// Tạo thư mục outputex1 nếu chưa tồn tại
			File outputDir = new File("outputex1");
			if (!outputDir.exists()) {
				outputDir.mkdir();
			}

			for (String mode : modes) {
				System.out.println("Chế độ: " + mode);

				// Gọi hàm thực hiện mã hóa và giải mã cho chế độ hiện tại
				processMode(mode, secretKey, ivSpec, inputFile, "outputex1/output_" + mode.replace("/", "_"));
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	// Hàm thực hiện mã hóa và giải mã cho một chế độ cụ thể
	private static void processMode(String mode, SecretKey secretKey, IvParameterSpec ivSpec, String inputFile,
			String outputFileBase) throws Exception {
		// Tên file kết quả
		String outputEncFile = outputFileBase + ".enc";
		String outputDecFile = outputFileBase + ".dec";

		// Mã hóa
		long startTime = System.currentTimeMillis();
		encryptWithMode(mode, secretKey, ivSpec, inputFile, outputEncFile);
		long endTime = System.currentTimeMillis();
		System.out.println("Thời gian mã hóa: " + (endTime - startTime) + " ms");

		// Giải mã
		startTime = System.currentTimeMillis();
		decryptWithMode(mode, secretKey, ivSpec, outputEncFile, outputDecFile);
		endTime = System.currentTimeMillis();
		System.out.println("Thời gian giải mã: " + (endTime - startTime) + " ms");

		System.out.println("Hoàn tất mã hóa và giải mã với chế độ: " + mode);
	}

	// Hàm mã hóa cho chế độ cụ thể
	private static void encryptWithMode(String mode, SecretKey secretKey, IvParameterSpec ivSpec, String inputFile,
			String outputFile) throws Exception {
		Cipher encryptCipher = Cipher.getInstance(mode);
		if (mode.contains("CBC")) {
			encryptCipher.init(Cipher.ENCRYPT_MODE, secretKey, ivSpec);
		} else {
			encryptCipher.init(Cipher.ENCRYPT_MODE, secretKey);
		}
		encryptFile(encryptCipher, inputFile, outputFile);
	}

	// Hàm giải mã cho chế độ cụ thể
	private static void decryptWithMode(String mode, SecretKey secretKey, IvParameterSpec ivSpec, String inputFile,
			String outputFile) throws Exception {
		Cipher decryptCipher = Cipher.getInstance(mode);
		if (mode.contains("CBC")) {
			decryptCipher.init(Cipher.DECRYPT_MODE, secretKey, ivSpec);
		} else {
			decryptCipher.init(Cipher.DECRYPT_MODE, secretKey);
		}
		decryptFile(decryptCipher, inputFile, outputFile);
	}

	private static SecretKey generateOrReadKey(String keyFile) throws Exception {
		try (FileInputStream fis = new FileInputStream(keyFile)) {
			byte[] keyBytes = new byte[8];
			fis.read(keyBytes);
			return new SecretKeySpec(keyBytes, "DES");
		} catch (Exception e) {
			// Tạo khóa mới nếu không đọc được từ file
			KeyGenerator keyGen = KeyGenerator.getInstance("DES");
			SecretKey secretKey = keyGen.generateKey();
			try (FileOutputStream fos = new FileOutputStream(keyFile)) {
				fos.write(secretKey.getEncoded());
			}
			return secretKey;
		}
	}

	private static void encryptFile(Cipher cipher, String inputFile, String outputFile) throws Exception {
		File file = new File(inputFile);
		long fileSize = file.length();

		try (InputStream in = new FileInputStream(inputFile);
				OutputStream out = new FileOutputStream(outputFile)) {
			byte[] buffer = new byte[64];
			int bytesRead;

			while ((bytesRead = in.read(buffer)) != -1) {
				if (bytesRead % 8 != 0 && cipher.getAlgorithm().contains("NoPadding")) {
					int paddingLength = 8 - (bytesRead % 8);
					byte[] paddedBuffer = new byte[bytesRead + paddingLength];
					System.arraycopy(buffer, 0, paddedBuffer, 0, bytesRead);
					for (int i = bytesRead; i < paddedBuffer.length; i++) {
						paddedBuffer[i] = 0; // Thêm padding với ký tự `0`
					}
					buffer = paddedBuffer;
					bytesRead = paddedBuffer.length;
				}

				byte[] output = cipher.update(buffer, 0, bytesRead);
				if (output != null) {
					out.write(output);
				}
			}
			byte[] output = cipher.doFinal();
			if (output != null) {
				out.write(output);
			}
		}
	}

	private static void decryptFile(Cipher cipher, String inputFile, String outputFile) throws Exception {
		try (InputStream in = new FileInputStream(inputFile);
				OutputStream out = new FileOutputStream(outputFile)) {
			byte[] buffer = new byte[64];
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
