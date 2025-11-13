<template>
	<div class="app">

		<!-- SIDE PANEL -->
		<side-panel 
			:categories="simpleCategories" 
			:selectedCategoryId="selectedCategoryId"
			@select-category="selectedCategoryId = $event"
			@add-category="addCategory"
			@delete-category="deleteCategory"
		 />

		<!-- CATEGORY VIEW -->
		<div v-if="loaded" class="category-view">
			<div class="top-bar">
				<simple-input-box
					v-model="searchQuery"
					placeholder="Search"
				/>
				<drop-down-dynamic-select 
					v-model="selectedSort"
					placeholder="No sorting"
					:select_options="sortOptions"
				/>
			</div>

			<item-list
				:items="filteredBySearch" 
				:category="categories.find(c => c.id === selectedCategoryId)"
				@delete-item="deleteItem"
				@add-item="addItem"
				@update-item="updateItem"
			/>
			
		</div>
	</div>
</template>


<script>
import ItemList from '@/components/ItemList'
import SidePanel from '@/components/SidePanel'

export default {
	components: {
		ItemList,
		SidePanel
	},
	data() {
		return {
			selectedCategoryId: 0,
			categories: [],
			serverUrl: 'http://localhost:8081',
			selectedSort: '',
			nextCategoryId: 0,
			nextItemId: 0,
			loaded: false,
			sortOptions: [
				//{value: 'creationTimeAsc', label: 'Creation Time Ascending'}, 
				//{value: 'creationTimeDesc', label: 'Creation Time Descending'}, 
				{value: 'titleAsc', label: 'Title Ascending'}, 
				{value: 'titleDesc', label: 'Title Descending'}, 
				//{value: 'ratingAsc', label: 'Rating Ascending'}, 
				//{value: 'ratingDesc', label: 'Rating Descending'}, 
				//{value: 'expectationAsc', label: 'Expectation Ascending'}, 
				//{value: 'expectationDesc', label: 'Expectation Descending'}
			],
			searchQuery: ''
		}
	},
	methods: {
		async fetchItems(categoryId) {
			try {
				const requestOptions = {
					categoryIds: [categoryId],
					withItems: true
				}
				const response = await fetch(`${this.serverUrl}/api/categories`, {
					method: 'POST',
					headers: {
						'Content-Type': 'application/json'
					},
					body: JSON.stringify({
						...requestOptions
					})
				})
				const data = await response.json()
				if(data.categories.length > 0){
					const cIndex = this.categories.findIndex(c => c.id === categoryId)
					if(cIndex !== -1){
						this.categories[cIndex] = data.categories[0]
					}
				}
			} catch (error) {
				console.error('Error fetching items:', error)
			}
		},
		async fetchCategories() {
			try {
				const response = await fetch(`${this.serverUrl}/api/categories`, {
					method: 'POST',
					headers: {
						'Content-Type': 'application/json'
					},
					body: JSON.stringify({
						categoryIds: 'all',
						nextIds: true
					})
				})
				const data = await response.json()
				this.categories = data.categories
				this.nextCategoryId = data.nextCategoryId
				this.nextItemId = data.nextItemId
				await Promise.all(
					this.categories.map(category => this.fetchItems(category.id))
				)

				this.selectedCategoryId = this.categories[0].id
				this.loaded = true

			} catch (error) {
				console.error('Error fetching categories:', error)
			}
		},
		async addCategory(category) {
			category.id = this.nextCategoryId
			this.categories.push(category)
			this.nextCategoryId++
			const response = await fetch(`${this.serverUrl}/api/categories/add`, {
				method: 'POST',
				headers: {
					'Content-Type': 'application/json'
				},
				body: JSON.stringify({
					category: {
						id: category.id,
						name: category.name,
						items: [],
						removed: category.removed
					},
					nextCategoryId: this.nextCategoryId
				})
			})
			const data = await response.json()
			if(data.categories.length > 0){
				const cIndex = this.categories.findIndex(c => c.id === data.categories[0].id)
				if(cIndex !== -1){
					this.categories[cIndex].id = data.categories[0].id
					this.categories[cIndex].name = data.categories[0].name
					this.categories[cIndex].removed = data.categories[0].removed
				}
			}
		},
		async addItem(item) {
			const cIndex = this.categories.findIndex(c => c.id === this.selectedCategoryId)
			if(cIndex !== -1){
				item.id = this.nextItemId
				this.categories[cIndex].items.push(item)
				this.nextItemId++
				const response = await fetch(`${this.serverUrl}/api/items/add`, {
					method: 'POST',
					headers: {
						'Content-Type': 'application/json'
					},
					body: JSON.stringify({
						categoryId: this.selectedCategoryId,
						item: item,
						nextItemId: this.nextItemId
					})
				})
				const data = await response.json()
				if(data.categories.length > 0){
					const cIndex = this.categories.findIndex(c => c.id === this.selectedCategoryId)
					if(cIndex !== -1){
						this.categories[cIndex] = data.categories[0]
					}
				}
			}
		},
		updateItem(item) {
			const cIndex = this.categories.findIndex(c => c.id === this.selectedCategoryId)
			if(cIndex !== -1){
				this.categories[cIndex].items = this.categories[cIndex].items.map(i => i.id === item.id ? item : i)
			}
		},
		deleteItem(item) {
			const cIndex = this.categories.findIndex(c => c.id === this.selectedCategoryId)
			if(cIndex !== -1){
				this.categories[cIndex].items = this.categories[cIndex].items.filter(i => i.id !== item.id)
			}
		},
		async deleteCategory(category) {
			const response = await fetch(`${this.serverUrl}/api/categories/delete`, {
				method: 'POST',
				headers: {
					'Content-Type': 'application/json'
				},
				body: JSON.stringify({
					categoryId: category.id
				})
			})
			const data = await response.json()
			if(data.categories.length > 0){
				const cIndex = this.categories.findIndex(c => c.id === data.categories[0].id)
				if(cIndex !== -1){
					this.categories[cIndex] = {
						id: data.categories[0].id,
						name: data.categories[0].name,
						items: this.categories[cIndex].items,
						removed: data.categories[0].removed
					}
				}
			}
			this.selectedCategoryId = this.categories[this.categories.findIndex(c => !c.removed)].id
		}
	},
	mounted() {
		this.fetchCategories()
	},
	computed: {
		sortedItems() {
			if(this.loaded){
				const cIndex = this.categories.findIndex(c => c.id === this.selectedCategoryId)
				if(cIndex !== -1){
					return [...this.categories[cIndex].items].sort((a, b) => {
						let sortOption = this.selectedSort
						if(sortOption === 'titleAsc') {
							return a.title.localeCompare(b.title)
						} else if(sortOption === 'titleDesc') {
							return b.title.localeCompare(a.title)
						}
					})
				}
			}
			return []
		},
		filteredBySearch() {
			if(this.loaded){
				return this.sortedItems.filter(item => item.title.toLowerCase().includes(this.searchQuery.toLowerCase()))
			}
			return []
		},
		simpleCategories() {
			if(this.loaded){
				return this.categories.map(category => ({
					id: category.id,
					name: category.name,
						items: category.items.length,
						removed: category.removed
					}))
			}
			return []
		}
	}
}
</script>

<style>
:root {
	--big-border-rad: 8px;
	--small-border-rad: 5px;
	--dark-bg-col: #141010;
	--accent-col: #d4ea9a;
	--accent-col-hover: #c1debb;
	--main-text-col: #f4f4df;
	--secondary-text-col: #888888;
	--option-bg-col: #242424;
	--strong-font: 'Montserrat', 'Segoe UI', sans-serif;
	--main-font: 'Poiret One', 'Segoe UI', sans-serif;
	--main-font-weight: 500;
	--planned-col: #784e92;
	--started-col: #5f68a0;
	--complete-col: #378182;
	--dropped-col: #96723d;
}


* {
	margin: 0;
	padding: 0;
	box-sizing: border-box;
}

body {
	color: var(--main-text-col);
	background-color: var(--dark-bg-col);
	font-family: var(--main-font);
	font-weight: var(--main-font-weight);

}

.app{
	display: flex;
	flex-direction: row;
	padding: 20px;
	gap: 20px;
}

.category-view{
	flex: 1;
}

.base-input{
	padding: 10px;
	border: 1px solid var(--accent-col);
	border-radius: var(--small-border-rad);
	background-color: var(--dark-bg-col);
	color: var(--main-text-col);
	font-family: var(--main-font);
	font-weight: var(--main-font-weight);
}

.base-button{
	padding: 10px 15px;
	border: 1px solid var(--accent-col);
	border-radius: var(--small-border-rad);
	background: none;
	color: var(--main-text-col);
	font-family: var(--main-font);
	font-weight: var(--main-font-weight);
}

.base-dialog{
	position: fixed;
	top: 0;
	left: 0;
	bottom: 0;
	right: 0;
	background: rgba(255, 255, 255, 0.2);
	display: flex;
}

.base-input:hover{
	background-color: rgba(255, 255, 255, 0.1);
	cursor: pointer;
	transition: background-color 0.1s ease;
}

.top-bar{
	display: flex;
	justify-content: space-between;
	margin-bottom: 20px;
	align-items: center;
}

.base-select{
	padding: 10px;
	border: 1px solid var(--accent-col);
	border-radius: var(--small-border-rad);
	background: none;
	color: var(--main-text-col);
	font-family: var(--main-font);
	font-weight: var(--main-font-weight);

}

.base-select:hover{
	background-color: rgba(255, 255, 255, 0.1);
	border-color: var(--accent-col-hover);
	cursor: pointer;
	transition: background-color 0.1s ease, border-color 0.1s ease;
}

button:hover{
	color: var(--accent-col-hover);
	background-color: rgba(255, 255, 255, 0.1);
	border-color: var(--accent-col-hover);
	cursor: pointer;
	transition: color 0.1s ease, background-color 0.1s ease, border-color 0.1s ease;
}

</style>