package com.brevex.ParkingApp.controller;

import com.brevex.ParkingApp.service.FirebaseAuthentication;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

@RestController
@RequestMapping("/auth")
public class AuthController
{
    private final FirebaseAuthentication firebaseAuthentication;

    @Autowired
    public AuthController(FirebaseAuthentication firebaseAuthentication)
    {
        this.firebaseAuthentication = firebaseAuthentication;
    }

    @PostMapping("/login")
    public String login(@RequestParam String email, @RequestParam String password)
    {
        return firebaseAuthentication.authenticateUser(email, password);
    }
}