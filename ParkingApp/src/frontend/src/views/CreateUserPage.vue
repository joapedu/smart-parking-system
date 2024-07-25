<template>
  <div>
    <h1>Create User</h1>
    <form @submit.prevent="createUser">
      <input v-model="email" type="email" placeholder="Email" required />
      <input v-model="password" type="password" placeholder="Password" required />
      <button type="submit">Create</button>
    </form>
  </div>
</template>

<script>
import api from '../services/api'

export default
{
  data() {
    return {
      email: '',
      password: ''
    }
  },
  methods: {
    async createUser()
    {
      try
      {
        await api.post('/create', {email: this.email, password: this.password})
        this.$router.push('/login')
      }
      catch (error)
      {
        console.error('Erro ao criar usuário:', error)
      }
    }
  }
}
</script>
