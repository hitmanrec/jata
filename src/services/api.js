const API_BASE_URL = 'http://localhost:8081'

async function apiRequest(endpoint, options = {}) {

	const url = `${API_BASE_URL}${endpoint}`;
	const defaultOptions = {
		method: 'POST',
		headers: { 
			'Content-Type': 'application/json'
		}
	}

	const config = {
		...defaultOptions,
		...options,
		headers: {
			...defaultOptions.headers,
			...(options.headers || {})
		}
	}

	if(options.body){
		config.body = JSON.stringify(options.body);
	}

	try {
		const controller = new AbortController();
		const timeout = setTimeout(() => controller.abort(), 10000);
		const response = await fetch(url, config);
		clearTimeout(timeout);

		if(!response.ok){
			throw new Error(`HTTP error! status: ${response.status}`);
		}

		const data = await response.json();
		return data;
	} catch (error) {
		console.error(`API error (${endpoint}):`, error);
		throw error;
	}
}

export const categoryAPI = {
	async getAllCategories(includeNextIds = true, removed = false) {
		return apiRequest('/api/categories', {
			body: {
				categoryIds: 'all',
				nextIds: includeNextIds,
				removed: removed
			}
		});
	},
	
	async getCategoriesByIds(categoryIds, withItems = true, removed = false) {
		return apiRequest('/api/categories', {
			body: {
				categoryIds: Array.isArray(categoryIds) ? categoryIds : [categoryIds],
				withItems: withItems,
				removed: removed
			}
		});
	},

	async addCategory(category, nextCategoryId) {
		return apiRequest('/api/categories/add', {
			body: {
				category: {
					id: category.id,
					name: category.name,
					items: [],
					removed: category.removed || false
				},
				nextCategoryId: nextCategoryId
			}
		});
	},

	async deleteCategory(categoryId) {
		return apiRequest('/api/categories/delete', {
			body: {
				categoryId: categoryId
			}
		});
	},

	async updateCategory(category) {
		return apiRequest('/api/categories/update', {
			body: {
				category: {
					id: category.id,
					name: category.name,
				}
			}
		});
	}
}

export const itemAPI = {
	async addItem(item, categoryId, nextItemId) {
		return apiRequest('/api/items/add', {
			body: {
				categoryId: categoryId,
				item: item,
				nextItemId: nextItemId
			}
		});
	},

	async updateItem(item, categoryId) {
		return apiRequest('/api/items/update', {
			body: {
				categoryId: categoryId,
				item: item
			}
		});
	},

	async deleteItem(itemId, categoryId) {
		return apiRequest('/api/items/delete', {
			body: {
				itemId: itemId,
				categoryId: categoryId
			}
		});
	}
	
}

export default {
	categoryAPI,
	itemAPI
}