package com.brevex.ParkingApp.controller;

import com.brevex.ParkingApp.infra.security.TokenService;
import com.brevex.ParkingApp.model.User;
import com.brevex.ParkingApp.service.FirebaseAuthManager;
import com.brevex.ParkingApp.service.FirebaseUserManager;
import lombok.Getter;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

@RestController
@RequestMapping("/auth")
public class AuthController
{
    private final FirebaseAuthManager firebaseAuthManager;
    private final FirebaseUserManager firebaseUserManager;
    private final TokenService tokenService;

    @Autowired
    public AuthController(FirebaseAuthManager firebaseAuthManager,
                          FirebaseUserManager firebaseUserManager,
                          TokenService tokenService)
    {
        this.firebaseAuthManager = firebaseAuthManager;
        this.firebaseUserManager = firebaseUserManager;
        this.tokenService = tokenService;
    }

    @PostMapping("/login")
    public ResponseEntity<?> login(@RequestParam String email, @RequestParam String password)
    {
        User user = firebaseAuthManager.authenticateUser(email, password);
        String jwtToken = tokenService.generateToken(user);
        return ResponseEntity.ok(new AuthResponse(jwtToken));
    }

    @PostMapping("/create")
    public ResponseEntity<String> createUser(@RequestParam String email, @RequestParam String password)
    {
        String userId = firebaseUserManager.createUser(email, password);
        return ResponseEntity.ok("User created with ID: " + userId);
    }

    @DeleteMapping("/delete")
    public ResponseEntity<String> deleteUser(@RequestHeader("Authorization") String authHeader)
    {
        String token = authHeader.substring(7);
        String email = tokenService.validateToken(token);
        firebaseUserManager.deleteUser(email);
        return ResponseEntity.ok("User deleted successfully");
    }
}

@Getter
class AuthResponse
{
    private final String token;

    public AuthResponse(String token)
    {
        this.token = token;
    }
}