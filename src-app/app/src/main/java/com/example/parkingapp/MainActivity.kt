package com.example.parkingapp

import android.os.Bundle
import android.widget.Toast
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.ViewCompat
import androidx.core.view.WindowInsetsCompat
import com.example.parkingapp.databinding.ActivityMainBinding
import com.google.firebase.database.DatabaseReference
import com.google.firebase.database.FirebaseDatabase

class MainActivity : AppCompatActivity()
{
    private lateinit var binding: ActivityMainBinding
    private lateinit var database: DatabaseReference

    override fun onCreate(savedInstanceState: Bundle?)
    {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        binding.buttonRead.setOnClickListener { readData() }

        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.main)) { v, insets ->
            val systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars())
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom)
            insets
        }
    }

    private fun readData()
    {
        database = FirebaseDatabase.getInstance().getReference("logs")
        database.child("id").get().addOnSuccessListener {
            if (it.exists())
            {
                val id: String = it.value.toString()
                Toast.makeText(this, "Successful ID read", Toast.LENGTH_SHORT).show()
            }
            else
            {
                Toast.makeText(this, "/logs/id path does not exist", Toast.LENGTH_SHORT).show()
            }
        }.addOnFailureListener { Toast.makeText(this, "FAILED", Toast.LENGTH_SHORT).show() }
    }
}