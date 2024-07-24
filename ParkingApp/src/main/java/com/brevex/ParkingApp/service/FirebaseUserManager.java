package com.brevex.ParkingApp.service;

import com.brevex.ParkingApp.utils.enums.FirebaseEndpoint;
import io.github.cdimascio.dotenv.Dotenv;
import lombok.NonNull;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpEntity;
import org.springframework.http.HttpMethod;
import org.springframework.http.ResponseEntity;
import org.springframework.stereotype.Service;
import org.springframework.web.client.HttpStatusCodeException;
import org.springframework.web.client.RestTemplate;

import java.util.Map;
import java.util.HashMap;

@Service
public class FirebaseUserManager extends FirebaseSetupService
{
    @Autowired
    public FirebaseUserManager(RestTemplate restTemplate, Dotenv dotenv)
    {
        super(restTemplate, dotenv);
    }

    public String createUser(@NonNull String email, @NonNull String password)
    {
        String url = buildUrl(FirebaseEndpoint.SIGN_UP);
        HttpEntity<Map<String, String>> entity = createAuthRequestEntity(email, password);

        try
        {
            ResponseEntity<String> response = restTemplate.exchange(
                    url,
                    HttpMethod.POST,
                    entity,
                    String.class
            );
            return response.getBody();
        }
        catch (HttpStatusCodeException e)
        {
            throw new RuntimeException("Failed to create user: " + e.getStatusCode(), e);
        }
    }

    public void deleteUser(@NonNull String idToken)
    {
        String url = buildUrl(FirebaseEndpoint.DELETE_ACCOUNT);

        Map<String, String> body = new HashMap<>();
        body.put("idToken", idToken);

        HttpEntity<Map<String, String>> entity = createRequestEntity(body);

        try
        {
            restTemplate.exchange(
                    url,
                    HttpMethod.POST,
                    entity,
                    String.class
            );
        }
        catch (HttpStatusCodeException e)
        {
            throw new RuntimeException("Failed to delete user: " + e.getStatusCode(), e);
        }
    }
}
