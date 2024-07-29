<template>
  <div class="form-container">
    <form @submit.prevent="createUser" class="form">
      <router-link to="/" class="userIcon">
        <img src="@/assets/icons/user.png" alt="User Icon"/>
      </router-link>

      <input v-model="email" type="email" placeholder="Email" required class="input" />
      <input v-model="confirmEmail" type="email" placeholder="Confirm Email" required class="input" />
      <input v-model="password" type="password" placeholder="Password" required class="input" />
      <input v-model="confirmPassword" type="password" placeholder="Confirm Password" required class="input" />

      <p v-if="errorMessage" class="error-message">{{ errorMessage }}</p>
      <button type="submit" class="button">Sign Up</button>

      <div class="form-text">
        <p>Already have an account?</p>
        <router-link to="/" class="form-link">Sign in here</router-link>
      </div>
    </form>
  </div>
</template>

<script>
import { ref } from 'vue';
import { useRouter } from 'vue-router';
import api from '../services/api';

export default
{
  setup()
  {
    const email = ref('');
    const confirmEmail = ref('');
    const password = ref('');
    const confirmPassword = ref('');
    const errorMessage = ref('');
    const router = useRouter();

    const createUser = async () => {
      if (email.value !== confirmEmail.value)
      {
        errorMessage.value = 'Emails do not match';
        return;
      }

      if (password.value !== confirmPassword.value)
      {
        errorMessage.value = 'Passwords do not match';
        return;
      }

      try
      {
        await api.post('/create', null, {
          params: { email: email.value, password: password.value }
        });

        await router.push('/parking-info');
      }
      catch (error)
      {
        console.error('Erro ao criar usuário:', error);
      }
    };
    return { email, confirmEmail, password, confirmPassword, createUser, errorMessage };
  }
};
</script>

<style scoped>
.error-message
{
  color: red;
  margin-top: 1rem;
}

.form
{
  display: flex;
  flex-direction: column;
  align-items: center;
  background-color: #17052A;
  border-radius: 10px;
  margin-bottom: 1.5rem;
  padding: 2rem;
}

.input
{
  padding: 1rem;
  margin-top: 1rem;
  border-radius: 5px;
  border: 1px solid #ccc;
  background-color: white;
  width: 14rem;
}

.button
{
  padding: 1rem;
  border: none;
  border-radius: 5px;
  background-color: #080010;
  font-weight: bolder;
  color: white;
  cursor: pointer;
  margin-top: 1.5rem;
}

.button:hover
{
  background-color: #330066;
}

.form-text
{
  color: white;
  margin-top: 1rem;
  text-align: center;
}

.form-link
{
  color: #fff;
  font-weight: bold;
}
</style>
