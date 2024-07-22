package com.brevex.ParkingApp.service;

import com.brevex.ParkingApp.model.User;
import com.brevex.ParkingApp.utils.exceptions.AuthenticationServiceException;
import com.brevex.ParkingApp.utils.enums.FirebaseEndpoint;
import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.ObjectMapper;
import lombok.NonNull;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.core.ParameterizedTypeReference;
import org.springframework.http.HttpEntity;
import org.springframework.http.HttpMethod;
import org.springframework.http.ResponseEntity;
import org.springframework.stereotype.Service;
import org.springframework.web.client.RestTemplate;

import java.util.List;
import java.util.Map;

@Service
public class FirebaseAuthManager extends FirebaseSetupService
{
    @Autowired
    public FirebaseAuthManager(RestTemplate restTemplate)
    {
        super(restTemplate);
    }

    public User authenticateUser(String email, String password)
    {
        try
        {
            String url = buildUrl(FirebaseEndpoint.SIGN_IN);
            HttpEntity<Map<String, String>> entity = createAuthRequestEntity(email, password);

            ResponseEntity<Map<String, Object>> response = restTemplate.exchange(
                    url,
                    HttpMethod.POST,
                    entity,
                    new ParameterizedTypeReference<>() {}
            );

            Map<String, Object> responseBody = response.getBody();

            assert responseBody != null;

            String idToken = (String) responseBody.get("idToken");

            return new User(idToken, email, password);
        }
        catch (Exception e)
        {
            throw new AuthenticationServiceException("Failed to authenticate user");
        }
    }

    public String validateToken(@NonNull String idToken)
    {
        try
        {
            String url = "https://identitytoolkit.googleapis.com/v1/accounts:lookup?key=" + firebaseApiKey;
            Map<String, String> body = Map.of("idToken", idToken);
            HttpEntity<Map<String, String>> entity = createRequestEntity(body);

            ResponseEntity<Map<String, Object>> response = restTemplate.exchange(
                    url,
                    HttpMethod.POST,
                    entity,
                    new ParameterizedTypeReference<>() {}
            );

            ObjectMapper mapper = new ObjectMapper();

            Map<String, Object> responseBody = response.getBody();

            assert responseBody != null;

            List<Map<String, Object>> users = mapper.convertValue(
                    responseBody.get("users"),
                    new TypeReference<>() {}
            );

            if (users != null && !users.isEmpty())
            {
                return (String) users.getFirst().get("email");
            }
            else
            {
                throw new AuthenticationServiceException("Invalid or expired token");
            }
        }
        catch (Exception e)
        {
            throw new AuthenticationServiceException("Failed to validate token");
        }
    }
}