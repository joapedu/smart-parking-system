import axios from 'axios';

const DATABASE_URL = "";

export const getRealtimeData = async (path) => {
    try
    {
        const response = await axios.get(`${DATABASE_URL}${path}.json`);
        return response.data;
    }
    catch (error)
    {
        console.error(`Erro ao obter dados de ${path}:`, error);
        return null;
    }
};
