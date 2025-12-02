<template>
	<div class="app">

		<!-- SIDE PANEL -->
		<side-panel 
			:categories="simpleCategories" 
			:selectedCategoryId="selectedCategoryId"
			@select-category="selectedCategoryId = $event"
			@add-category="addCategory"
			@update-category="updateCategory"
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
				:category="categories.find(c => c.id === selectedCategoryId) || {name: '', id: 0, removed: false, items: []}"
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
import { categoryAPI, itemAPI } from '@/services/api'

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
				const itemsData = await categoryAPI.getCategoriesByIds([categoryId])
				if(itemsData.categories.length > 0){
					const cIndex = this.categories.findIndex(c => c.id === categoryId)
					if(cIndex !== -1){
						this.categories[cIndex] = itemsData.categories[0]
					}
				}
			} catch (error) {
				console.error('Error fetching items:', error)

				alert('Failed to fetch items. Please try again later.')
			}
		},
		async fetchCategories() {
			try {
				const CategoriesData = await categoryAPI.getAllCategories(true)

				this.categories = CategoriesData.categories
				this.nextCategoryId = CategoriesData.nextCategoryId
				this.nextItemId = CategoriesData.nextItemId

				await Promise.all(
					this.categories.map(category => this.fetchItems(category.id))
				)
				if(this.categories.length > 0){
					this.selectedCategoryId = this.categories[0].id
				} else {
					this.selectedCategoryId = 0
				}
				this.loaded = true

			} catch (error) {
				console.error('Error fetching categories:', error)

				alert('Failed to fetch categories. Please try again later.')
			}
		},
		async addCategory(category) {
			try {
				category.id = this.nextCategoryId
				this.nextCategoryId++
				this.categories.push(category)
				
				const newCategoryData = await categoryAPI.addCategory({id: category.id, name: category.name, removed: category.removed}, this.nextCategoryId)

				if(newCategoryData.category){
					const cIndex = this.categories.findIndex(c => c.id === newCategoryData.category.id)
					if(cIndex !== -1){
						this.categories[cIndex] = {
							id: this.categories[cIndex].id,
							name: newCategoryData.category.name,
							removed: false,
							items: this.categories[cIndex].items
						}
						this.selectedCategoryId = this.categories[cIndex].id
					} else {
						this.selectedCategoryId = 0
					}
					this.nextCategoryId = newCategoryData.nextCategoryId
				}
			} catch (error) {
				console.error('Error adding category:', error)
				alert('Failed to update category. Please try again later.')
			}
		},
		async updateCategory(category) {
			try {
				const CategoriesData = await categoryAPI.updateCategory(category)
				if(CategoriesData.category){
					const cIndex = this.categories.findIndex(c => c.id === CategoriesData.category.id)
					if(cIndex !== -1){
						this.categories[cIndex] = {
							id: this.categories[cIndex].id,
							name: CategoriesData.category.name,
							removed: this.categories[cIndex].removed,
							items: this.categories[cIndex].items
						}
					}
				}
			} catch (error) {
				console.error('Error updating category:', error)
				alert('Failed to update category. Please try again later.')
			}
		},
		async deleteCategory(category) {
			try {
				const cIndex = this.categories.findIndex(c => c.id === category.id)
				if(cIndex !== -1){
					this.categories[cIndex].removed = true
				} else {
					throw new Error('Category not found')
				}
				const CategoriesData = await categoryAPI.deleteCategory(category.id)
				if(CategoriesData.category){
					const cdcIndex = this.categories.findIndex(c => c.id === CategoriesData.category.id)
					if(cdcIndex !== -1){
						this.categories.splice(cdcIndex, 1)
					}
				}
				const scIndex = this.categories.findIndex(c => c.id !== CategoriesData.category.id);
				if(scIndex !== -1){
					this.selectedCategoryId = this.categories[scIndex].id
				} else {
					this.selectedCategoryId = 0
				}
			} catch (error) {
				console.error('Error deleting category:', error)
				alert('Failed to delete category. Please try again later.')
			}
		},
		async addItem(item, categoryId) {
			try {
				item.id = this.nextItemId
				this.nextItemId++
				const cIndex = this.categories.findIndex(c => c.id === categoryId)
				if(cIndex !== -1){
					this.categories[cIndex].items.push(item)
				} else {
					throw new Error('Category not found')
				}
				const ItemsData = await itemAPI.addItem(item, categoryId, this.nextItemId)
				if(ItemsData.item){
					const idcIndex = this.categories.findIndex(c => c.id === ItemsData.categoryId)
					let iIndex = -1;
					if(idcIndex !== -1){
						iIndex = this.categories[idcIndex].items.findIndex(i => i.id === ItemsData.item.id)
					} else {
						throw new Error('Category not found')
					}
					if(idcIndex !== -1 && iIndex !== -1){
						this.categories[idcIndex].items[iIndex] = ItemsData.item
					} else {
						throw new Error('Item not found')
					}
					this.nextItemId = ItemsData.nextItemId
				}
			} catch (error) {
				console.error('Error adding item:', error)
				alert('Failed to add item. Please try again later.')
			}
		},
		async updateItem(item, categoryId) {
			try {
				const cIndex = this.categories.findIndex(c => c.id === categoryId)
				let iIndex = -1;
				if(cIndex !== -1){
					iIndex = this.categories[cIndex].items.findIndex(i => i.id === item.id)
				} else {
					throw new Error('Category not found')
				}
				if(cIndex !== -1 && iIndex !== -1){
					this.categories[cIndex].items[iIndex] = item
					const ItemsData = await itemAPI.updateItem(item, categoryId, this.nextItemId)
					if(ItemsData.item){
						this.categories[cIndex].items[iIndex] = ItemsData.item
					}
				} else {
					throw new Error('Item not found')
				}
			} catch (error) {
				console.error('Error updating item:', error)
				alert('Failed to update item. Please try again later.')
			}
		},
		async deleteItem(item, categoryId) {
			try {
				if(!confirm('Are you sure you want to delete this item?')){
					return
				}
				const itemId = item.id
				const cIndex = this.categories.findIndex(c => c.id === categoryId)
				let iIndex = -1;
				if(cIndex != -1){
					iIndex = this.categories[cIndex].items.findIndex(i => i.id === itemId)
				} else {
					throw new Error('Category not found')
				}
				if(iIndex !== -1){
					this.categories[cIndex].items[iIndex].removed = true
				} else {
					throw new Error('Item not found')
				}
				const ItemsData = await itemAPI.deleteItem(itemId, categoryId)
				if(ItemsData.itemId){
					this.categories[cIndex].items.splice(iIndex, 1)
				}
			} catch (error) {
				console.error('Error deleting item:', error)
				alert('Failed to delete item. Please try again later.')
			}
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
						return 0
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