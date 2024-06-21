package com.example.parkingsystem.controller;

import com.example.parkingsystem.service.FirebaseService;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;

@Controller
public class ViewController
{
    private final FirebaseService firebaseService;

    public ViewController(FirebaseService firebaseService)
    {
        this.firebaseService = firebaseService;
    }

    @GetMapping("/")
    public String index(Model model) throws InterruptedException
    {
        model.addAttribute("status", firebaseService.getStatus());
        model.addAttribute("logs", firebaseService.getLogs());
        return "index";
    }
}
