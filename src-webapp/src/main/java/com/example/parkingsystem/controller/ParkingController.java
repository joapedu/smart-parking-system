package com.example.parkingsystem.controller;

import com.example.parkingsystem.service.FirebaseService;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
@RequestMapping("/api/parking")
public class ParkingController
{
    private final FirebaseService firebaseService;

    public ParkingController(FirebaseService firebaseService)
    {
        this.firebaseService = firebaseService;
    }

    @GetMapping("/status")
    public String getStatus() throws InterruptedException
    {
        return firebaseService.getStatus();
    }

    @GetMapping("/logs")
    public String getLogs() throws InterruptedException
    {
        return firebaseService.getLogs();
    }
}
