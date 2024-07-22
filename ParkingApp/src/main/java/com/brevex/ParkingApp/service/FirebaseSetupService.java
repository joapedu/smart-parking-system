package com.brevex.ParkingApp.service;

import com.brevex.ParkingApp.utils.enums.FirebaseEndpoint;
import io.github.cdimascio.dotenv.Dotenv;
import jakarta.annotation.PostConstruct;
import org.springframework.http.HttpEntity;
import org.springframework.http.HttpHeaders;
import org.springframework.http.MediaType;
import org.springframework.stereotype.Service;
import org.springframework.web.client.RestTemplate;

import java.util.HashMap;
import java.util.Map;

@Service
public abstract class FirebaseSetupService
{
    protected String firebaseApiKey;
    protected final RestTemplate restTemplate;
    protected HttpHeaders headers;

    public FirebaseSetupService(RestTemplate restTemplate)
    {
        this.restTemplate = restTemplate;
        Dotenv dotenv = Dotenv.load();
        this.firebaseApiKey = dotenv.get("FIREBASE_API_KEY");
    }

    @PostConstruct
    public void init()
    {
        this.headers = new HttpHeaders();
        this.headers.setContentType(MediaType.APPLICATION_JSON);
    }

    protected String buildUrl(FirebaseEndpoint endpoint)
    {
        return "https://identitytoolkit.googleapis.com/v1/"
                + endpoint.getEndpoint()
                + "?key=" + firebaseApiKey;
    }

    protected HttpEntity<Map<String, String>> createRequestEntity(Map<String, String> body)
    {
        return new HttpEntity<>(body, headers);
    }

    protected HttpEntity<Map<String, String>> createAuthRequestEntity(String email, String password)
    {
        Map<String, String> body = new HashMap<>();
        body.put("email", email);
        body.put("password", password);
        body.put("returnSecureToken", "true");

        return createRequestEntity(body);
    }
}
