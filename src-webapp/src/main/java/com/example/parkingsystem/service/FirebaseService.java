package com.example.parkingsystem.service;

import com.google.firebase.database.*;
import org.springframework.stereotype.Service;

import javax.annotation.PostConstruct;
import java.util.concurrent.CountDownLatch;

@Service
public class FirebaseService
{
    private DatabaseReference databaseReference;

    @PostConstruct
    private void init()
    {
        databaseReference = FirebaseDatabase.getInstance().getReference("parking_system");
    }

    public String getStatus() throws InterruptedException
    {
        final CountDownLatch latch = new CountDownLatch(1);
        final String[] status = new String[1];

        databaseReference.child("status").addListenerForSingleValueEvent(new ValueEventListener()
        {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot)
            {
                status[0] = dataSnapshot.getValue(String.class);
                latch.countDown();
            }

            @Override
            public void onCancelled(DatabaseError databaseError)
            {
                latch.countDown();
            }
        });

        latch.await();
        return status[0];
    }

    public String getLogs() throws InterruptedException
    {
        final CountDownLatch latch = new CountDownLatch(1);
        final String[] logs = new String[1];

        databaseReference.child("logs").addListenerForSingleValueEvent(new ValueEventListener()
        {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot)
            {
                logs[0] = dataSnapshot.getValue(String.class);
                latch.countDown();
            }

            @Override
            public void onCancelled(DatabaseError databaseError)
            {
                latch.countDown();
            }
        });

        latch.await();
        return logs[0];
    }
}
