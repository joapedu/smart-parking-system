package com.brevex.ParkingApp.infra.config;

import io.github.cdimascio.dotenv.Dotenv;
import org.springframework.boot.web.client.RestTemplateBuilder;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.web.client.RestTemplate;

@Configuration
public class AppConfig
{
    private final Dotenv dotenv;

    public AppConfig(Dotenv dotenv)
    {
        this.dotenv = dotenv;
    }

    @Bean
    public RestTemplate restTemplate(RestTemplateBuilder builder)
    {
        return builder
                .rootUri("https://identitytoolkit.googleapis.com/v1")
                .build();
    }

    @Bean
    public String firebaseApiKey()
    {
        return dotenv.get("FIREBASE_API_KEY");
    }

    @Bean
    public String jwtSecret()
    {
        return dotenv.get("JWT_SECRET");
    }
}
