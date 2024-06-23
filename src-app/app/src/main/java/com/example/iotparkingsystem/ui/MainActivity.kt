package com.example.iotparkingsystem.ui

import android.os.Bundle
import androidx.activity.viewModels
import androidx.appcompat.app.AppCompatActivity
import androidx.lifecycle.Observer
import com.example.iotparkingsystem.R
import com.example.iotparkingsystem.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity()
{
    private lateinit var binding: ActivityMainBinding
    private val viewModel: MainViewModel by viewModels()

    override fun onCreate(savedInstanceState: Bundle?)
    {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        viewModel.parkingData.observe(this, Observer { data ->
            binding.tvVagasDisponiveis.text = getString(R.string.vagas_disponiveis, data.vagasDisponiveis)
            binding.tvUltimoID.text = getString(R.string.ultimo_id, data.ultimoID)
            binding.tvAlarmAtivacoes.text = getString(R.string.alarm_ativacoes, data.alarmAtivacoes)
            binding.tvUltimoIDAlarme.text = getString(R.string.ultimo_id_alarme, data.ultimoIDAlarme)
            binding.tvWifiConectado.text = getString(R.string.wifi_conectado, if (data.wifiConectado) getString(R.string.wifi_sim) else getString(R.string.wifi_nao))
        })
    }
}
