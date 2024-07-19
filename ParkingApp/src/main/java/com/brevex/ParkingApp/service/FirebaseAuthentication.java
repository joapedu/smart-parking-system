package com.brevex.ParkingApp.service;

import com.brevex.ParkingApp.service.exceptions.AuthenticationServiceException;
import io.github.cdimascio.dotenv.Dotenv;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.core.ParameterizedTypeReference;
import org.springframework.http.HttpEntity;
import org.springframework.http.HttpHeaders;
import org.springframework.http.MediaType;
import org.springframework.http.ResponseEntity;
import org.springframework.stereotype.Service;
import org.springframework.web.client.RestTemplate;

import java.net.URI;
import java.net.URISyntaxException;
import java.util.HashMap;
import java.util.Map;

@Service
public class FirebaseAuthentication
{
    private final URI firebaseAuthUrl;
    private final RestTemplate restTemplate;
    private final HttpHeaders headers;

    @Autowired
    public FirebaseAuthentication(Dotenv dotenv, RestTemplate restTemplate) throws URISyntaxException
    {
        String apiKey = dotenv.get("FIREBASE_API_KEY");

        this.firebaseAuthUrl = new URI("https://identitytoolkit.googleapis.com/v1/accounts:signInWithPassword?key=" + apiKey);
        this.restTemplate = restTemplate;
        this.headers = new HttpHeaders();
        this.headers.setContentType(MediaType.APPLICATION_JSON);
    }

    public String authenticateUser(String email, String password)
    {
        Map<String, String> body = new HashMap<>();

        body.put("email", email);
        body.put("password", password);
        body.put("returnSecureToken", "true");

        HttpEntity<Map<String, String>> request = new HttpEntity<>(body, headers);

        ResponseEntity<Map<String, Object>> response = restTemplate.exchange(
                firebaseAuthUrl,
                org.springframework.http.HttpMethod.POST,
                request,
                new ParameterizedTypeReference<>() {}
        );

        if (response.getStatusCode().is2xxSuccessful())
        {
            Map<String, Object> responseBody = response.getBody();
            assert responseBody != null;
            return (String) responseBody.get("idToken");
        }
        else
        {
            throw new AuthenticationServiceException("Failed to authenticate user");
        }
    }
}