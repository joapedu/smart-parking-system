package com.example.iotparkingsystem

import android.annotation.SuppressLint
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import android.widget.TextView
import android.widget.Toast
import com.google.firebase.database.FirebaseDatabase
import com.google.firebase.database.DatabaseReference
import com.google.firebase.database.DataSnapshot
import com.google.firebase.database.DatabaseError
import com.google.firebase.database.ValueEventListener

class MainActivity : AppCompatActivity()
{
    private lateinit var database: DatabaseReference

    override fun onCreate(savedInstanceState: Bundle?)
    {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        database = FirebaseDatabase.getInstance().getReference()

        database.addValueEventListener(object : ValueEventListener
        {
            @SuppressLint("SetTextI18n")
            override fun onDataChange(snapshot: DataSnapshot)
            {
                val vagasDisponiveis = snapshot.child("vagasDisponiveis").getValue(Int::class.java) ?: 0
                val ultimoID = snapshot.child("ultimoID").getValue(String::class.java) ?: ""
                val alarmAtivacoes = snapshot.child("alarmAtivacoes").getValue(Int::class.java) ?: 0
                val ultimoIDAlarme = snapshot.child("ultimoIDAlarme").getValue(String::class.java) ?: ""
                val wifiConectado = snapshot.child("wifiConectado").getValue(Boolean::class.java) ?: false

                findViewById<TextView>(R.id.tvVagasDisponiveis).text = "Vagas Disponíveis: $vagasDisponiveis"
                findViewById<TextView>(R.id.tvUltimoID).text = "Último ID: $ultimoID"
                findViewById<TextView>(R.id.tvAlarmAtivacoes).text = "Ativações de Alarme: $alarmAtivacoes"
                findViewById<TextView>(R.id.tvUltimoIDAlarme).text = "Último ID Alarme: $ultimoIDAlarme"
                findViewById<TextView>(R.id.tvWifiConectado).text = "WiFi Conectado: ${if (wifiConectado) "Sim" else "Não"}"
            }

            override fun onCancelled(error: DatabaseError)
            {
                Toast.makeText(baseContext, "Falha ao ler dados.", Toast.LENGTH_SHORT).show()
            }
        })
    }
}
